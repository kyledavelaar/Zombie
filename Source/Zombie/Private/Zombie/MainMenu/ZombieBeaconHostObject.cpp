// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/MainMenu/ZombieBeaconHostObject.h"
// #include "Zombie/Public/Zombie/MainMenu/ZombieBeaconHostObject.h"
#include "Zombie/MainMenu/ZombieBeaconClient.h"
#include "Zombie/MainMenu/ZombieMainMenuGameMode.h"
#include "OnlineBeaconHost.h"
#include "TimerManager.h"

AZombieBeaconHostObject::AZombieBeaconHostObject()
{
	ClientBeaconActorClass = AZombieBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AZombieBeaconHostObject::UpdateLobbyInfo(FZombieLobbyInfo NewLobbyInfo)
{
	LobbyInfo.MapImage = NewLobbyInfo.MapImage;
	UpdateClientLobbyInfo();
	FOnHostLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconHostObject::UpdateClientLobbyInfo()
{
	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
	{
		if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
		{
			Client->Client_OnLobbyUpdated(LobbyInfo);
		}
	}
}

void AZombieBeaconHostObject::BeginPlay()
{
	LobbyInfo.PlayerList.Add(FString("Host"));

	GetWorld()->GetTimerManager().SetTimer(TInitialLobbyHandle, this, &AZombieBeaconHostObject::InitialLobbyHandling, 0.2f, false);

}


void AZombieBeaconHostObject::InitialLobbyHandling()
{
	UpdateLobbyInfo(LobbyInfo);
}

void AZombieBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{ 
	Super::OnClientConnected(NewClientActor, ClientConnection);

	if (NewClientActor)
	{
		FString PlayerName = FString("Player ");
		int8 Index = LobbyInfo.PlayerList.Num();
		PlayerName.Append(FString::FromInt(Index));
		LobbyInfo.PlayerList.Add(PlayerName);

		if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(NewClientActor))
		{
			Client->SetPlayerIndex(Index);
			Client->SetPlayerName(PlayerName);
		}

		FOnHostLobbyUpdated.Broadcast(LobbyInfo);

		UE_LOG(LogTemp, Warning, TEXT("CONNECTED CLIENT VALID"));
		UpdateClientLobbyInfo(); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CONNECTED CLIENT INVALID"));
	}
}

void AZombieBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);
	UE_LOG(LogTemp, Warning, TEXT("CLIENT HAS DISCONNECTED")); 

	if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(LeavingClientActor))
	{
		uint8 Index = Client->GetPlayerIndex();
		LobbyInfo.PlayerList.RemoveAt(Index);
	}
	FOnHostLobbyUpdated.Broadcast(LobbyInfo);
	UpdateClientLobbyInfo();
}

void AZombieBeaconHostObject::ShutdownServer()
{
	// unregister server from db via web api
	DisconnectAllClients();

	if (AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("DESTROYING HOST BEACON"));
		Host->UnregisterHost(BeaconTypeName);
		Host->DestroyBeacon();
	}
}

void AZombieBeaconHostObject::DisconnectAllClients()
{
	UE_LOG(LogTemp, Warning, TEXT("DISCONNECTING ALL CLIENTS"));

	for (AOnlineBeaconClient* Client : ClientActors) {
		if (Client)
		{
			DisconnectClient(Client);
		}
	}
}

void AZombieBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
	AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());
	if (BeaconHost)
	{
		if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("DISCONNECTING CLIENT %s"), *ClientActor->GetName());
			Client->Client_OnDisconnected();
		}
		BeaconHost->DisconnectClient(ClientActor);
	}

}

// #3 Host/server sends messages to all clients
void AZombieBeaconHostObject::SendChatToLobby(const FText& ChatMessage)
{
	FOnHostChatReceived.Broadcast(ChatMessage);

	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
	{
		if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
		{
			Client->Client_OnChatMessageReceived(ChatMessage);
		}
	}
}
