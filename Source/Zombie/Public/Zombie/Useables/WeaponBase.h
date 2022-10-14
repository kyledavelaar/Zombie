// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class ZOMBIE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class UAnimMontage* FPSArmsFireMontage;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		int32 MagazineMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		int32 BaseDamage;

	int32 CurrentTotalAmmo;
	int32 CurrentMagazineAmmo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FHitResult> PerformLineTrace(class AZombiePlayerCharacter* ShootingPlayer);

public:
	// virtual void Fire() = 0; is a pure virtual function, no definition required in WeaponBase.cpp file
	virtual TArray<FHitResult> Fire(class AZombiePlayerCharacter* ShootingPlayer);
	virtual void Reload();

	// first element is CurrentMagazineAmmo, second element is CurrentTotalAmmo
	TArray<int32> GetCurrentAmmo();

	class UAnimMontage* GetFireAnimMontage();
};
