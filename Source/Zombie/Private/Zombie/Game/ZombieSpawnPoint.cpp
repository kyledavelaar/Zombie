// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Game/ZombieSpawnPoint.h"
#include "Zombie/Useables/Barricade.h"

AZombieSpawnPoint::AZombieSpawnPoint()
{
	Zone = 0;
	bIsActive = false;
}

void AZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

}

ABarricade* AZombieSpawnPoint::GetLinkedBarricade()
{
	return LinkedBarricade;
}

uint8 AZombieSpawnPoint::GetZone()
{
	return Zone;
}

void AZombieSpawnPoint::SetZone(uint8 NewZone)
{
	Zone = NewZone;
}

bool AZombieSpawnPoint::IsActive()
{
	return bIsActive;
}

void AZombieSpawnPoint::Activate()
{
	bIsActive = true;
}
