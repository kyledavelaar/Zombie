// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ZombieSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombieSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	AZombieSpawnPoint();

protected:
	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class ABarricade* LinkedBarricade;

	uint8 Zone;
	bool bIsActive;

	virtual void BeginPlay() override;

public:
	class ABarricade* GetLinkedBarricade();
	uint8 GetZone();
	void SetZone(uint8 NewZone);
	bool IsActive();
	void Activate();
};
