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

	//����׷�ٺ��Ե�Actors
	TArray<AActor*> IgnoreActors;
protected:
	virtual void BeginPlay()override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION()
	void OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//����ͼ�д���˲ʱ������������������Ʒ
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);
private:
	//ֻ�����ص��¼��ĺ�����ײ��
	UPROPERTY(VisibleAnywhere,Category="Weapon Property")
	UBoxComponent* WeaponBox;

	/*
	* �������к���׷�ٵ���ʼ����յ㣬�ҵ���ȷ����ײ��
	*/
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox()const { return WeaponBox; }
};
