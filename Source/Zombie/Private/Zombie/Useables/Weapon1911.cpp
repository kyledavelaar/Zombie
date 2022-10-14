// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/Weapon1911.h"
#include "Player/ZombiePlayerCharacter.h"
#include "Zombie/ZombieCharacter/ZombieBase.h"

#include "Components/SkeletalMeshComponent.h"

AWeapon1911::AWeapon1911()
{
	WeaponName = "1911";
	BaseDamage = 100;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

TArray<FHitResult> AWeapon1911::Fire(AZombiePlayerCharacter* ShootingPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING 1911"));

	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);


	// set visibility to overlap on mesh collision so can shoot through objects
	// add channel in project settings collision section, then go to DefaultEngine.ini config to find what name is: ECC_GameTraceChannel2
	if (HitResults.Num() > 0)
	{
		for (FHitResult& HitResult : HitResults)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
					{
						Zombie->Hit(ShootingPlayer);
					}
				UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
			}
		}
	}
	return HitResults;

}

void AWeapon1911::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("RELOADING 1911"));
}
