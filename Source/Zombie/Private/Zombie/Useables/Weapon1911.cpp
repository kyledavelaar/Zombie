// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/Weapon1911.h"
#include "Player/ZombiePlayerCharacter.h"
#include "Zombie/ZombieCharacter/ZombieBase.h"

#include "Components/SkeletalMeshComponent.h"

AWeapon1911::AWeapon1911()
{
	WeaponName = "1911";
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

// server gets message from client that client fired
void AWeapon1911::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	UE_LOG(LogTemp, Warning, TEXT("Server fire function"));
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

TArray<FHitResult> AWeapon1911::Fire(AZombiePlayerCharacter* ShootingPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING 1911"));

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


	return TArray<FHitResult>();

}

void AWeapon1911::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("RELOADING 1911"));
}
