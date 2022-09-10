// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/MainMenu/ZombieBeaconClient.h"
// #include "Zombie/Public/Zombie/MainMenu/ZombieBeaconClient.h"

AZombieBeaconClient::AZombieBeaconClient()
{
	PlayerIndex = 0;
}

bool AZombieBeaconClient::ConnectToServer(const FString& Address)
{
	FURL Destination = FURL(nullptr, *Address, ETravelType::TRAVEL_Absolute);
	Destination.Port = 7787; // matches port # in our config
	return InitClient(Destination);
}

void AZombieBeaconClient::OnConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT CONNECTED TO HOST BEACON"));
	FOnConnected.Broadcast(true);
}

void AZombieBeaconClient::OnFailure()
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT FAILED TO CONNECT TO HOST BEACON"));
	FOnConnected.Broadcast(false);
}

// #1 client sends message (called from blueprint)
void AZombieBeaconClient::SendChatMessage(const FText& ChatMessage)
{
	//FString Message = ChatMessage.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("Chat From Client: %s"), *Message);

	// calls server rpc
	Server_SendChatMessage(ChatMessage); 
}

bool AZombieBeaconClient::Server_SendChatMessage_Validate(const FText& ChatMessage)
{
	return true;
}


// #2 Server RPC
void AZombieBeaconClient::Server_SendChatMessage_Implementation(const FText& ChatMessage)
{
	FString Message = PlayerName + ": " + ChatMessage.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Chat in Server: %s"), *Message);

	// BeaconOwner is on BeaconHostObject, which we get access to b/c we import ZombieBeaconHostObject which extends BeaconHostObject
	if (AZombieBeaconHostObject* Host = Cast<AZombieBeaconHostObject>(BeaconOwner))
	{
		Host->SendChatToLobby(FText::FromString(Message));
	}
}

void AZombieBeaconClient::Client_OnDisconnected_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("DISCONNECTED"));
	FOnDisconnected.Broadcast();
}

void AZombieBeaconClient::Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo)
{
	FOnLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconClient::Client_OnChatMessageReceived_Implementation(const FText& ChatMessage)
{
	FOnChatReceived.Broadcast(ChatMessage);
}

void AZombieBeaconClient::SetPlayerIndex(uint8 Index)
{
	PlayerIndex = Index;
}

uint8 AZombieBeaconClient::GetPlayerIndex()
{
	return PlayerIndex;
}

void AZombieBeaconClient::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

FString AZombieBeaconClient::GetPlayerName()
{
	return PlayerName;
}


void AZombieBeaconClient::LeaveLobby()
{
	DestroyBeacon();
}