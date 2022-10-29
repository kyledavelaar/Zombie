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
	if (CurrentMagazineAmmo > 0)
	{
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
					if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
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
	Super::Fire(ShootingPlayer);
	if (CurrentMagazineAmmo > 0)
	{
		if (FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}

		// set visibility to overlap on mesh collision so can shoot through objects
		// add channel in project settings collision section, then go to DefaultEngine.ini config to find what name is: ECC_GameTraceChannel2
		if (GetWorld()->IsServer())
		{
			TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

			if (HitResults.Num() > 0)
			{
				for (FHitResult& HitResult : HitResults)
				{
					FString HitBone = HitResult.BoneName.ToString();
					UE_LOG(LogTemp, Warning, TEXT("Bone Hit: %s"), *HitBone);
					if (AActor* HitActor = HitResult.GetActor())
					{
						if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
						{
							Zombie->Hit(ShootingPlayer, HitResult);
						}
						UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
					}
				}
			}

		}
		else
		{
			TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

			if (HitResults.Num() > 0)
			{
				for (FHitResult& HitResult : HitResults)
				{
					if (AActor* HitActor = HitResult.GetActor())
					{
						if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
						{
							Zombie->Hit(ShootingPlayer, HitResult);
						}
						UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
					}
				}
			}
			// send client hit result to server
			Server_Fire(HitResults);
		}
		return true;
	}

	return false;

}

void AWeaponSemi::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("RELOADING 1911"));
}

