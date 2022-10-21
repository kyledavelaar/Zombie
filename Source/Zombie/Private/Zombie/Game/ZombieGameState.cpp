// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombieGameState.h"

AZombieGameState::AZombieGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
	TotalZombiesRemaining = 5;
}

uint16 AZombieGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombieGameState::IncrementRoundNumber()
{
	RoundNumber++;
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
}
