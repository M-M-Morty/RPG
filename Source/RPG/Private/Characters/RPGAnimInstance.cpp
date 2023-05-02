// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGAnimInstance.h"
#include "Characters/RPGCharacter.h"
#include "Kismet//KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character=Cast<ARPGCharacter>(TryGetPawnOwner());

	if (Character)
	{
		RPGCharacterMovement = Character->GetCharacterMovement();
	}
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (RPGCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(RPGCharacterMovement->Velocity);
		IsFalling = RPGCharacterMovement->IsFalling();
		CharacterState = Character->GetCharacterState();
	}


}
