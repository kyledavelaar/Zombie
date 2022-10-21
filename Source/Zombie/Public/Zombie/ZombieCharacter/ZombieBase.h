// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieBase.generated.h"

UCLASS()
class ZOMBIE_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	uint8 GetHitPart(FString BoneName);
	uint8 GetPointsForHit(uint8 HitPart);
	void DecrementHealth(int16 Damage);
	void Die();


protected:
	int16 Health;

	UPROPERTY(EditDefaultsOnly)
		float CleanupDelay;

	UPROPERTY(ReplicatedUsing = OnRep_Die, EditAnywhere)
		bool bIsDead;
	UFUNCTION()
		void OnRep_Die();
	void OnCleanup();

public:
	void Hit(class AZombiePlayerCharacter* Player, FHitResult HitResult);
};
