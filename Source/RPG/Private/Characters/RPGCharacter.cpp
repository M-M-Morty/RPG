// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

ARPGCharacter::ARPGCharacter()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh());
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARPGCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ARPGCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ARPGCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARPGCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ARPGCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ARPGCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ARPGCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Disarm"), IE_Pressed, this, &ARPGCharacter::Disarm);
	PlayerInputComponent->BindAction(FName("ArmFirst"), IE_Pressed, this, &ARPGCharacter::ArmFirst);
}

void ARPGCharacter::MoveForward(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;
	if (Controller)
	{
		const FRotator ControRotation = GetControlRotation();
		const FRotator YawRotation = FRotator(0.f, ControRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
 
void ARPGCharacter::MoveRight(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;
	if (Controller)
	{
		const FRotator ControRotation = GetControlRotation();
		const FRotator YawRotation = FRotator(0.f, ControRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARPGCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ARPGCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Attacking) return;
	ACharacter::Jump();
}

void ARPGCharacter::EKeyPressed()
{
	AWeapon* OverlapItem = Cast<AWeapon>(OverlappingItem);
	if (OverlapItem)
	{
		OverlapItem->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquipFirstWeapon = OverlapItem;
	}
}

void ARPGCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ARPGCharacter::Disarm()
{
	if (CanDisarm())
	{
		PlayEquipMontage(FName("Unequip")); 
		CharacterState = ECharacterState::ECS_Unequipped;
		UE_LOG(LogTemp, Warning, TEXT("Disarm"));
	}
}

void ARPGCharacter::ArmFirst()
{
	if (CanArm())
	{
		PlayEquipMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		UE_LOG(LogTemp, Warning, TEXT("Arm"));
	}
}

void ARPGCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionType)
{
	if (EquipFirstWeapon && EquipFirstWeapon->GetWeaponBox())
	{
		EquipFirstWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionType);
		EquipFirstWeapon->IgnoreActors.Empty();
	}
}

bool ARPGCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_UnOccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}


bool ARPGCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_UnOccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARPGCharacter::CanArm()
{
	return ActionState == EActionState::EAS_UnOccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquipFirstWeapon; 
}

void ARPGCharacter::WeaponDisarm()
{
	if (EquipFirstWeapon)
	{
		EquipFirstWeapon->AttachToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ARPGCharacter::WeaponArm()
{
	if (EquipFirstWeapon)
	{
		EquipFirstWeapon->AttachToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ARPGCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = "Attack2";
			break;
		case 1:
			SectionName = "Attack2";
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}


void ARPGCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ARPGCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}



