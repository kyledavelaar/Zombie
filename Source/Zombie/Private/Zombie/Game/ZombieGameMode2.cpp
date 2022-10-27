// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombieGameMode2.h"
#include "Zombie/ZombieCharacter/ZombieBase.h"
#include "Zombie/Game/ZombiePlayerSpawnPoint.h"
#include "Zombie/Useables/Barricade.h"
#include "Zombie/Game/ZombieSpawnPoint.h"
#include "Zombie/Public/Player/ZombiePlayerCharacter.h"
#include "Zombie/Game/ZombieGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


AZombieGameMode2::AZombieGameMode2()
{
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_ZombiePlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	bHasLoadedSpawnPoints = false;

	ZombiesRemaining = 0;


}

void AZombieGameMode2::BeginPlay()
{
	Super::BeginPlay();

	ZombieGameState = GetGameState<AZombieGameState>();
	CalculateZombieCount();

	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieSpawnPoint::StaticClass(), TempActors);
	
	for (AActor* Actor : TempActors)
	{
		if (AZombieSpawnPoint* SpawnPoint = Cast<AZombieSpawnPoint>(Actor))
		{
			if (ABarricade* LinkedBarricade = SpawnPoint->GetLinkedBarricade())
			{
				// zombie spawn points that will be used later in the game once player has opened doors to new sections of the map
				SpawnPoint->SetZone(LinkedBarricade->GetAccessZone());
				ZombieSpawnPoints.Add(SpawnPoint);
				UE_LOG(LogTemp, Warning, TEXT("linked zone, %d"), LinkedBarricade->GetAccessZone());
			}
			else
			{
				// default spawn point not linked to any barricades (i.e. when game starts before any doors openened to other zones)
				ActiveZombieSpawnPoints.Add(SpawnPoint);
			}
		}
	
	}

	GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AZombieGameMode2::SpawnZombie, 1.0f, true);
	GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
	CalculateZombieCount();
}

void AZombieGameMode2::SetSpawnPoints()
{
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombiePlayerSpawnPoint::StaticClass(), TempActors);

	for (AActor* Actor : TempActors)
	{
		if (AZombiePlayerSpawnPoint* SpawnPoint = Cast<AZombiePlayerSpawnPoint>(Actor))
		{
			PlayerSpawnPoints.Add(SpawnPoint);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawn Point Count: %d"), PlayerSpawnPoints.Num());
	bHasLoadedSpawnPoints = true;
}

void AZombieGameMode2::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!bHasLoadedSpawnPoints)
	{
		SetSpawnPoints();
	}

	for (AZombiePlayerSpawnPoint* SpawnPoint : PlayerSpawnPoints)
	{
		if (!SpawnPoint->IsUsed())
		{
			FVector SpawnLocation = SpawnPoint->GetActorLocation();
			if (APawn* Pawn = GetWorld()->SpawnActor<APawn>(PlayerClass, SpawnLocation, FRotator::ZeroRotator))
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNED PAWN TO POSSESS")); 
				NewPlayer->Possess(Pawn);
				SpawnPoint->SetUsed(true);
				return;
			}
		}
	}
}

void AZombieGameMode2::NewZoneActive(uint8 ZoneNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("NEW ZONE SET, %d"), ZoneNumber);

	for (int16 x = ZombieSpawnPoints.Num() - 1; x >= 0; --x)
	{
		AZombieSpawnPoint* ZombieSpawnPoint = ZombieSpawnPoints[x];

		if (ZombieSpawnPoint && ZoneNumber == ZombieSpawnPoint->GetZone() && !ZombieSpawnPoint->IsActive())
		{
			ActiveZombieSpawnPoints.Add(ZombieSpawnPoint);
			ZombieSpawnPoint->Activate();
			ZombieSpawnPoints.RemoveAt(x);
		}
	}
}

void AZombieGameMode2::SpawnZombie()
{
	if (ZombieGameState)
	{
		uint8 PlayerCount = ZombieGameState->PlayerArray.Num();
		uint8 MaxZombiesOnMapAtOnce = 24;
		if (PlayerCount > 1)
			MaxZombiesOnMapAtOnce += PlayerCount * 6;

		if (ZombiesRemaining > 0 && ZombieGameState->GetZombiesOnMap() <= MaxZombiesOnMapAtOnce - 1)
		{
			int RandomIndex = FMath::RandRange(0, ActiveZombieSpawnPoints.Num() - 1);
			if (AZombieSpawnPoint* SpawnPoint = ActiveZombieSpawnPoints[RandomIndex])
			{
				FVector Location = SpawnPoint->GetActorLocation();
				FRotator Rotation = SpawnPoint->GetActorRotation();
				if (AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Location, Rotation))
				{
			
					ZombieGameState->ZombieSpawned();
					--ZombiesRemaining;
				}
			}
		}
		else if (ZombiesRemaining <= 0)
		{
			GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
		}
	}
}

void AZombieGameMode2::CalculateZombieCount()
{
	if (ZombieGameState)
	{
		uint16 RoundNumber = ZombieGameState->GetRoundNumber();
		uint8 PlayerCount = ZombieGameState->PlayerArray.Num();
		uint8 MaxZombiesOnMapAtOnce = 24;
		if (PlayerCount > 1)
			MaxZombiesOnMapAtOnce += PlayerCount * 6;

		if (RoundNumber > 0 && RoundNumber <= 5)
		{
			UE_LOG(LogTemp, Warning, TEXT("Round 1 - 5"));
			ZombiesRemaining = FMath::FloorToInt((RoundNumber * 0.2f) * MaxZombiesOnMapAtOnce);
			UE_LOG(LogTemp, Warning, TEXT("Zombies Remaining, %d"), ZombiesRemaining);
		}
		else
		{
			ZombiesRemaining = FMath::FloorToInt(RoundNumber * 0.15f * MaxZombiesOnMapAtOnce);
		}


		ZombieGameState->SetTotalZombiesRemaining(ZombiesRemaining);
		GetWorld()->GetTimerManager().UnPauseTimer(TZombieSpawnHandle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no game state"));
	}
}


void AZombieGameMode2::ZombieKilled()
{
	if (ZombieGameState)
	{
		ZombieGameState->ZombieKilled();
		if (ZombieGameState->GetTotalZombiesRemaining() == 0)
		{
			// start new round
			ZombieGameState->IncrementRoundNumber();
			FTimerHandle TempHandle;
			GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AZombieGameMode2::CalculateZombieCount, 5.0f, false);
		}
	}
}
