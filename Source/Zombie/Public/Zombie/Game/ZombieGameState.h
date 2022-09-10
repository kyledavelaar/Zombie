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
	uint16 RoundNumber;
	uint8 ZombiesOnMap;

public:
	uint16 GetRoundNumber();
	void IncrementRoundNumber();
};
