// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/Weapons/WeaponSemi.h"
#include "Player/ZombiePlayerCharacter.h"
#include "Zombie/ZombieCharacter/ZombieBase.h"

#include "Components/SkeletalMeshComponent.h"


AWeaponSemi::AWeaponSemi()
{
	WeaponName = "Default Name";
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}


// server gets message from client that client fired
void AWeaponSemi::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	UE_LOG(LogTemp, Warning, TEXT("Server fire function"));
	UE_LOG(LogTemp, Warning, TEXT("AmmoReplicated: %d"), CurrentMagazineAmmo);
	if (CurrentMagazineAmmo > 0)
	{
		Super::Server_Fire_Implementation(HitResults);
		if (FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}

		if (HitResults.Num() > 0)
		{
			for (FHitResult HitResult : HitResults)
			{ // do check to prevent cheating
				if (AActor* HitActor = HitResult.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
					{
						if (AZombiePlayerCharacter* Player = Cast<AZombiePlayerCharacter>(GetOwner()))
							Zombie->Hit(Player, HitResult);
					}
					UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
				}
			}
		}
	}
}

bool AWeaponSemi::Fire(AZombiePlayerCharacter* ShootingPlayer)
{
	if (CurrentMagazineAmmo > 0)
	{
		Super::Fire(ShootingPlayer);
		if (FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}

		// set visibility to overlap on mesh collision so can shoot through objects
		// add channel in project settings collision section, then go to DefaultEngine.ini config to find what name is: ECC_GameTraceChannel2
		
		TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);
		if (HitResults.Num() > 0)
		{
			for (FHitResult& HitResult : HitResults)
			{
				FString HitBone = HitResult.BoneName.ToString();
				UE_LOG(LogTemp, Warning, TEXT("Bone Hit: %s"), *HitBone);
				if (AActor* HitActor = HitResult.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
					{
						Zombie->Hit(ShootingPlayer, HitResult);
					}
					UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
				}
			}
		}
		if (!GetWorld()->IsServer())
		{
			// send client hit result to server
			Server_Fire(HitResults);
		}
		return true;
	}

	return false;

}

bool AWeaponSemi::Reload()
{
	if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
	{
		if (ReloadAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
		int Difference = MagazineMaxAmmo - CurrentMagazineAmmo;
		CurrentTotalAmmo -= Difference;
		CurrentMagazineAmmo = MagazineMaxAmmo;
		return true;
	}
	else
	{
		return false;
	}
}

