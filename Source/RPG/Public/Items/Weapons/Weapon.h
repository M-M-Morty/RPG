// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

class UBoxComponent;
class USceneComponent;

UCLASS()
class RPG_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	void Equip(USceneComponent *InParent,FName InSocketname);
	void AttachToSocket(USceneComponent* InParent, const FName& InSocketname);

	//盒型追踪忽略的Actors
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay()override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
	void OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//在蓝图中创造瞬时的物理场，用来打碎物品
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);
private:
	//只产生重叠事件的盒型碰撞体
	UPROPERTY(VisibleAnywhere,Category="Weapon Property")
	UBoxComponent* WeaponBox;

	/*
	* 用来进行盒型追踪的起始点和终点，找到精确的碰撞点
	*/
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox()const { return WeaponBox; }
};
