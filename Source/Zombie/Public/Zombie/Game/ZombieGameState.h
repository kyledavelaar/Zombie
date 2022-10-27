// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ZombieGameState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombieGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AZombieGameState();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
		uint16 RoundNumber; // set to replicate
	uint8 ZombiesOnMap; // set to replicate
	uint16 TotalZombiesRemaining; // set to replicate
	float ZombieHealth;

public:
	uint16 GetRoundNumber(); 
	void IncrementRoundNumber();
	void SetTotalZombiesRemaining(const uint16& ZombieCount);
	uint16 GetTotalZombiesRemaining();
	void ZombieKilled();
	uint8 GetZombiesOnMap();
	void ZombieSpawned();
	float GetZombieHealth();
};
