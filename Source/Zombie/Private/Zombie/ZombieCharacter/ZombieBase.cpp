// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/ZombieCharacter/ZombieBase.h"
#include "Zombie/Game/ZombieGameMode2.h"
#include "Zombie/Useables/WeaponBase.h"
#include "Zombie/Public/Player/ZombiePlayerCharacter.h"
#include "Zombie/Public/Player/ZombiePlayerState.h"
#include "Zombie/Game/ZombieGameState.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

AZombieBase::AZombieBase()
{
	Health = 150;
	bIsDead = false;
	CleanupDelay = 5.0f;
}

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (AZombieGameState* GS = GetWorld()->GetGameState<AZombieGameState>())
		{
			Health = GS->GetZombieHealth();
			UE_LOG(LogTemp, Warning, TEXT("Zombie Health: %f"), Health);
		}
	}
}

// replicate propeties down to clients
void AZombieBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZombieBase, bIsDead);
	DOREPLIFETIME(AZombieBase, Health);
}

uint8 AZombieBase::GetHitPart(FString BoneName)
{
	if (BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		return 1; // limb hit
	}
	else if (BoneName.Equals(FString("spine_01")) || BoneName.Equals(FString("pelvis")))
	{
		return 2;  // abdomen
	}
	else if (BoneName.Equals(FString("spine_02")))
	{
		return 3; // chest
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		return 4; // neck
	}
	else if (BoneName.Equals(FString("head")))
	{
		return 5; // head
	}
	return 0;
}

uint8 AZombieBase::GetPointsForHit(uint8 HitPart, float WeaponDamage)
{
	DecrementHealth(WeaponDamage);

	if (Health - WeaponDamage <= 0) // get bonus points if hit is on a death hit
	{
		switch (HitPart)
		{
			case 1: return 50;
			case 2: return 60;
			case 3: return 60;
			case 4: return 70;
			case 5: return 100;
			default: return 0;
		}
	}
	else // non-death hit
	{
		return 10;
	}
}

void AZombieBase::DecrementHealth(int16 Damage)
{
	if (HasAuthority())
	{
		Health -= Damage;
		if (Health <= 0)
		{
			Die();
		}
	}
}

void AZombieBase::Die()
{
	if (HasAuthority())
	{
		if (AZombieGameMode2* ZombieGameMode = GetWorld()->GetAuthGameMode<AZombieGameMode2>())
		{
			ZombieGameMode->ZombieKilled();
		}
		bIsDead = true;
		OnRep_Die();
	}
}

// should run on both client and server
void AZombieBase::OnRep_Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ragdoll effect
	GetMesh()->SetSimulatePhysics(true);
	
	if (HasAuthority())
	{
		if (AController* AIController = GetController<AController>())
		{
			AIController->Destroy();
		}
		FTimerHandle TempHandle;
		GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AZombieBase::OnCleanup, CleanupDelay, false);
	}
}

void AZombieBase::OnCleanup()
{
	Destroy();
}

void AZombieBase::Hit(AZombiePlayerCharacter* Player, FHitResult HitResult)
{
	if (Player && !bIsDead)
	{
		if (AZombiePlayerState* PState = Player->GetPlayerState<AZombiePlayerState>())
		{
			FString BoneName = HitResult.BoneName.ToString();
			if (BoneName == FString("None"))
			{
				return;
			}
			if (uint8 HitPart = GetHitPart(BoneName))
			{
				if (AWeaponBase* PlayerWeapon = Player->GetCurrentWeapon())
				{
					EHitLocation HitLocation = EHitLocation::None;
					switch (HitPart)
					{
						case 2: HitLocation = EHitLocation::Torso; break;
						case 3: HitLocation = EHitLocation::Chest; break;
						case 4:
						case 5:
							HitLocation = EHitLocation::Head; break;
					}
					float WeaponDamage = PlayerWeapon->GetWeaponDamage().GetDamage(HitLocation);
					UE_LOG(LogTemp, Warning, TEXT("Weapon damage: %f"), WeaponDamage);

					if (uint8 PointsForHit = GetPointsForHit(HitPart, WeaponDamage))
					{
						PState->IncrementPoints(PointsForHit);
					}

				}
			}
		}
	}
}


