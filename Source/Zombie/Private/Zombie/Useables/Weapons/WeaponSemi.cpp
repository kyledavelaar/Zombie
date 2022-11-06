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
}


void AWeaponSemi::BeginPlay()
{
	Super::BeginPlay();
	// can't be set in constructor or won't be able to change values in BP
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}



// server gets message from client that client fired
void AWeaponSemi::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	//UE_LOG(LogTemp, Warning, TEXT("Server fire function"));
	//UE_LOG(LogTemp, Warning, TEXT("AmmoReplicated: %d"), CurrentMagazineAmmo);
	if (CurrentMagazineAmmo > 0)
	{
		Super::Server_Fire_Implementation(HitResults);
		/*if (FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}*/

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
					//UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
				}
			}
			Multi_Fire(HitResults[0]);
		}
	}
}

// server sending message to all the clients
void AWeaponSemi::Multi_Fire_Implementation(const FHitResult& HitResult)
{
	if (AZombiePlayerCharacter* Pawn = Cast<AZombiePlayerCharacter>(GetOwner()))
	{
		// if you were the client who reloaded don't show animation twice b/c already showed it in Fire()
		if (!Pawn->IsLocallyControlled() && FireAnimation)
		{
			if (UAnimInstance* AnimInstance = Pawn->GetMesh1P()->GetAnimInstance())
			{
				if (FPSArmsFireMontage)
				{
					AnimInstance->Montage_Play(FPSArmsFireMontage);
					if (Pawn->GetIsAiming())
					{
						// play aiming animation. vs hipfire but we currently only have ads animation so not using this
					}
					AnimInstance->Montage_JumpToSection(FName("FireADS"), FPSArmsFireMontage);
				}
			}
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
	}
}

// client calls fire 
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
			if (GetWorld()->IsServer())
			{
				Multi_Fire(HitResults[0]); // send to all server's clients
			}
			else
			{
				Server_Fire(HitResults); // sends to server the client's hit result, and then server calls multi-cast.  This way clients never deal with each other but only with the server
			}
			return true;
		}
	}

	return false;

}

//bool AWeaponSemi::Reload()
//{
//	return Super::Reload();
//}

