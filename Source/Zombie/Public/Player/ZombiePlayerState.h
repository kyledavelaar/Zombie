// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZombiePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, NewPoints);

UCLASS()
class ZOMBIE_API AZombiePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AZombiePlayerState();

protected:
	UPROPERTY(BlueprintAssignable)
		FPointsChanged NewPoints;

protected:
	// can't make uint (unsigned int) b/c subtracting in DecrementPoints will never get negative and conditional is checking for negative values
	// GetLifetimeReplicatedProps in cpp file and Replicated flag here so can see point update on clients and server.  If didn't have this only server would see their points update
	UPROPERTY(ReplicatedUsing = OnRep_PointsChanged, EditDefaultsOnly)
		int32 Points;
	UFUNCTION()
		void OnRep_PointsChanged();
	
public:
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);

	UFUNCTION(BlueprintCallable)
		int32 GetPoints();
};
