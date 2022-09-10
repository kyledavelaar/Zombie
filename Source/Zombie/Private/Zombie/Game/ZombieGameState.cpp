// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombieGameState.h"

AZombieGameState::AZombieGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
}

uint16 AZombieGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombieGameState::IncrementRoundNumber()
{
	RoundNumber++;
}
