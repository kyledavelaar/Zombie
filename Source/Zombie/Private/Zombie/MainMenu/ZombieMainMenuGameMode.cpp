// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/MainMenu/ZombieMainMenuGameMode.h"
// #include "Zombie/Public/Zombie/MainMenu/ZombieMainMenuGameMode.h"
#include "Zombie/MainMenu/ZombieBeaconHostObject.h"
#include "OnlineBeaconHost.h"

AZombieMainMenuGameMode::AZombieMainMenuGameMode()
{
	HostObject = nullptr;
	Host = nullptr;
}

bool AZombieMainMenuGameMode::CreateHostBeacon()
{
	UE_LOG(LogTemp, Warning, TEXT("SPAWNED AONLINEBEACONHOST"));
	Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass());

	if (Host)
	{
		UE_LOG(LogTemp, Warning, TEXT("GOT HOST"));
		if (Host->InitHost())
		{
			UE_LOG(LogTemp, Warning, TEXT("INIT HOST"));
			Host->PauseBeaconRequests(false);
			HostObject = GetWorld()->SpawnActor<AZombieBeaconHostObject>(AZombieBeaconHostObject::StaticClass());

			if (HostObject)
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNED HOST OBJECT"));
				Host->RegisterHost(HostObject);
				return true;
			}
		}
	}
	return false;
}

AZombieBeaconHostObject* AZombieMainMenuGameMode::GetBeaconHost()
{
	return HostObject;
}
