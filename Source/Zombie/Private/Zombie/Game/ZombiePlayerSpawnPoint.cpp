// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombiePlayerSpawnPoint.h"

AZombiePlayerSpawnPoint::AZombiePlayerSpawnPoint()
{
	bIsUsed = false;
}

bool AZombiePlayerSpawnPoint::IsUsed()
{
	return bIsUsed;
}

void AZombiePlayerSpawnPoint::SetUsed(bool Used)
{
	bIsUsed = Used;
}
