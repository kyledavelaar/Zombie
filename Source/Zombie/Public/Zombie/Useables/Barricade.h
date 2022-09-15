// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie/Useables/InteractableBase.h"
#include "Barricade.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API ABarricade : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	ABarricade();

protected:
	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class UStaticMeshComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class UAnimationAsset* OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		uint16 Cost;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		uint8 AccessZone;

	UPROPERTY(ReplicatedUsing = OnRep_BarricadeUsed)
		bool bIsUsed;

	UFUNCTION()
		void OnRep_BarricadeUsed();

protected:
	virtual void BeginPlay() override;

public:
	uint8 GetAccessZone();
	virtual void Use(class AZombiePlayerCharacter* Player) override;
};
