// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AZombieMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZombieMainMenuGameMode();
	
protected:
	UFUNCTION(BlueprintCallable)
		bool CreateHostBeacon();

	class AZombieBeaconHostObject* HostObject;
	class AOnlineBeaconHost* Host;

	UFUNCTION(BlueprintCallable)
		class AZombieBeaconHostObject* GetBeaconHost();

};
