// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "RPG/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/RPGCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(OtherActor);
	if (RPGCharacter)
	{
		RPGCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(OtherActor);
	if (RPGCharacter)
	{
		RPGCharacter->SetOverlappingItem(nullptr);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}


/*打印字符的两种方式
	FString Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("DeltaTime:%s"), *Name);
	if (GEngine)
	{
		FString ItemName = FString::Printf(TEXT("DeltaTime:%s"), *Name);
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, ItemName);
	}
*/