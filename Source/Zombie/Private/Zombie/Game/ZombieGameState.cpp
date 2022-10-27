// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombieGameState.h"

AZombieGameState::AZombieGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
	TotalZombiesRemaining = 5;
	ZombieHealth = 150;
}

void AZombieGameState::BeginPlay()
{
	Super::BeginPlay();
}

uint16 AZombieGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombieGameState::IncrementRoundNumber()
{
	RoundNumber++;
	// zombie health increases on higher rounds
	if (RoundNumber < 10)
	{
		ZombieHealth += 100;
	}
	else
	{
		ZombieHealth *= 1.1f;
	}
}

void AZombieGameState::SetTotalZombiesRemaining(const uint16& ZombieCount)
{
	TotalZombiesRemaining = ZombieCount;
}

uint16 AZombieGameState::GetTotalZombiesRemaining()
{
	return TotalZombiesRemaining;
}

void AZombieGameState::ZombieKilled()
{
	--TotalZombiesRemaining;
	--ZombiesOnMap;
}

uint8 AZombieGameState::GetZombiesOnMap()
{
	return ZombiesOnMap;
}

void AZombieGameState::ZombieSpawned()
{
	++ZombiesOnMap;
}

float AZombieGameState::GetZombieHealth()
{
	return ZombieHealth;
}
