// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombieGameMode2.h"
#include "Zombie/ZombieCharacter/ZombieBase.h"
#include "Zombie/Game/ZombiePlayerSpawnPoint.h"
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
			ZombieSpawnPoints.Add(SpawnPoint);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Zombie Spawn Count %d"), ZombieSpawnPoints.Num());

	GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AZombieGameMode2::SpawnZombie, 2.0f, true);
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

void AZombieGameMode2::CalculateZombieCount()
{
	if (ZombieGameState)
	{
		uint16 RoundNumber = ZombieGameState->GetRoundNumber();
		// TODO: do calculation here
		ZombiesRemaining = 5;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no game state"));
	}
}

void AZombieGameMode2::SpawnZombie()
{
	if (ZombiesRemaining > 0)
	{
		int RandomIndex = FMath::RandRange(0, ZombieSpawnPoints.Num() - 1);
		if (AZombieSpawnPoint* SpawnPoint = ZombieSpawnPoints[RandomIndex])
		{
			FVector Location = SpawnPoint->GetActorLocation();
			FRotator Rotation = SpawnPoint->GetActorRotation();
			if (AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Location, Rotation))
			{
				UE_LOG(LogTemp, Warning, TEXT("spawning zombie"));
				--ZombiesRemaining;
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
	}
}