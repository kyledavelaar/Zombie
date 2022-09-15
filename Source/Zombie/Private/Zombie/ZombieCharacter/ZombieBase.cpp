// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/ZombieCharacter/ZombieBase.h"
#include "Zombie/Public/Player/ZombiePlayerCharacter.h"

AZombieBase::AZombieBase()
{


}

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieBase::Hit(AZombiePlayerCharacter* Player)
{
	if (HasAuthority() && Player)
	{
		Player->IncrementPoints(100);
	}
}


