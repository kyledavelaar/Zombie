// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombiePlayerState.h"
#include "Net/UnrealNetwork.h"

AZombiePlayerState::AZombiePlayerState()
{
	Points = 500;
}

// Replicate Points so can see updates on clients and server, wihtout this they would only appear on server
//void AZombiePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}

void AZombiePlayerState::OnRep_PointsChanged()
{
	NewPoints.Broadcast(Points);
}

void AZombiePlayerState::IncrementPoints(uint16 Value)
{
	
	Points += Value;
	if (HasAuthority())
		OnRep_PointsChanged();
	NewPoints.Broadcast(Points); // broadcasting to ourself only
	UE_LOG(LogTemp, Warning, TEXT("POINTS: %d"), Points);
}

bool AZombiePlayerState::DecrementPoints(uint16 Value)
{
	// TODO: Client not decrementing points when open door
	if (HasAuthority())
	{
		if ((Points - Value) < 0)
		{
			return false;
		}
		else
		{
			Points -= Value;
		}
		OnRep_PointsChanged();
		UE_LOG(LogTemp, Warning, TEXT("POINTS: %d"), Points);
	}
	return true;
}

int32 AZombiePlayerState::GetPoints()
{
	return Points;
}