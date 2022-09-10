// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ZombiePlayerSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombiePlayerSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	AZombiePlayerSpawnPoint();

protected:
	bool bIsUsed;

public:
	bool IsUsed();
	void SetUsed(bool Used);
};
