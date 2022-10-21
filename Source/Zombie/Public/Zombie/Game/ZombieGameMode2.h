// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieGameMode2.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombieGameMode2 : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZombieGameMode2();

protected:
	class AZombieGameState* ZombieGameState;

	bool bHasLoadedSpawnPoints;
	TArray<class AZombiePlayerSpawnPoint*> PlayerSpawnPoints;
	TArray<class AZombieSpawnPoint*> ActiveZombieSpawnPoints;
	TArray<class AZombieSpawnPoint*> ZombieSpawnPoints;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		TSubclassOf<class AZombiePlayerCharacter> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		TSubclassOf<class AZombieBase> ZombieClass;

	FTimerHandle TZombieSpawnHandle;

	uint16 ZombiesRemaining;

protected:
	void CalculateZombieCount();
	void SpawnZombie();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSpawnPoints();

public:
	void NewZoneActive(uint8 ZoneNumber);
	void ZombieKilled();
};
