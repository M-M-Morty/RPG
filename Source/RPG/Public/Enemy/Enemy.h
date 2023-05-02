// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Enemy.generated.h"


class UAnimMontage;
class USoundBase;
class UParticleSystem;

UCLASS()
class RPG_API AEnemy : public ACharacter,public IHitInterface
{
	GENERATED_BODY()
public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint)override;
	void DirectionalHitReact(const FVector& ImpactPoint);
protected:
	virtual void BeginPlay() override;
	/*
	*²¥·Å¶¯»­ÃÉÌ«Ææ
	*/
	void PlayHitReactMontage(const FName& SectionName);

private:
	/*
	*¶¯»­ÃÉÌ«Ææ
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditAnywhere,Category="Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* HitParticle;
};
