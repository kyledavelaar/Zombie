// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie/Useables/WeaponBase.h"
#include "Weapon1911.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIE_API AWeapon1911 : public AWeaponBase
{
	GENERATED_BODY()

public:
	AWeapon1911();

	virtual TArray<FHitResult> Fire(class AZombiePlayerCharacter* ShootingPlayer) override;
	virtual void Reload() override;
	
};
