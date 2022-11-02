// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie/Useables/WeaponBase.h"
#include "WeaponSemi.generated.h"


UCLASS()
class ZOMBIE_API AWeaponSemi : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponSemi();

protected:
	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults) override;

public:
	virtual bool Fire(class AZombiePlayerCharacter* ShootingPlayer) override;
	virtual bool Reload() override;
	
};
