// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/ZombieCharacter/ZombieBase.h"
#include "Zombie/Game/ZombieGameMode2.h"
#include "Zombie/Public/Player/ZombiePlayerCharacter.h"
#include "Zombie/Public/Player/ZombiePlayerState.h"
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
}

void AZombieBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZombieBase, bIsDead);
}

uint8 AZombieBase::GetHitPart(FString BoneName)
{
	if (BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		DecrementHealth(30);
		return 1; // limb hit
	}
	else if (BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{
		DecrementHealth(50);
		return 2; // torso
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		DecrementHealth(70);
		return 3; // neck
	}
	else if (BoneName.Equals(FString("head")))
	{
		DecrementHealth(90);
		return 4; // head
	}
	return 0;
}

uint8 AZombieBase::GetPointsForHit(uint8 HitPart)
{
	// get bonus points if hit is on a death hit
	if (Health - 50 <= 0)
	{
		switch (HitPart)
		{
			case 1: return 50;
			case 2: return 60;
			case 3: return 70;
			case 4: return 100;
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
	if (Player)
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
				if (uint8 PointsForHit = GetPointsForHit(HitPart))
				{
					PState->IncrementPoints(PointsForHit);
				}
			}
		}
	}
}


