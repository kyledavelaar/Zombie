// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum EHitLocation
{
	None UMETA(Displayname = "None"),
	Head UMETA(Displayname, "Head"),
	Chest UMETA(Displayname = "Chest"),
	Torso UMETA(Displayname, "Torso")
};

USTRUCT(BlueprintType)
struct FWeaponDamage
{
	GENERATED_BODY();
public:
	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly)
		float HeadMultiplier = 3.5f;
	UPROPERTY(EditDefaultsOnly)
		float ChestMultiplier = 1.25f;
	UPROPERTY(EditDefaultsOnly)
		float TorsoMultiplier = 1.1f;
	float GetDamage(EHitLocation HitLocation)
	{
		switch (HitLocation)
		{
			case Head: return BaseDamage * HeadMultiplier;
			case Chest: return BaseDamage * ChestMultiplier;
			case Torso: return BaseDamage * TorsoMultiplier;
			default: return BaseDamage;
		}
	}
};

UENUM(BlueprintType)
enum EWeaponID
{
	Colt1911 UMETA(Displayname, "1911"),
	M1Carbine UMETA(Displayname, "M1Carbine")
};

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
		TEnumAsByte<EWeaponID> WeaponID;

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
		FWeaponDamage WeaponDamage;

	UPROPERTY(Replicated)
		int32 CurrentTotalAmmo;
	UPROPERTY(Replicated)
		int32 CurrentMagazineAmmo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FHitResult> PerformLineTrace(class AZombiePlayerCharacter* ShootingPlayer);
	TArray<FHitResult> PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Fire(const TArray<FHitResult>& HitResults);
	bool Server_Fire_Validate(const TArray<FHitResult>& HitResults);
	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults);

public:
	// virtual void Fire() = 0; is a pure virtual function, no definition required in WeaponBase.cpp file
	virtual bool Fire(class AZombiePlayerCharacter* ShootingPlayer);
	virtual void Reload();
	FWeaponDamage GetWeaponDamage();

	// first element is CurrentMagazineAmmo, second element is CurrentTotalAmmo
	TArray<int32> GetCurrentAmmo();

	class UAnimMontage* GetFireAnimMontage();

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWeaponID> GetWeaponID();
};
