// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/Barricade.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABarricade::ABarricade()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	Cost = 500;
	ObjectName = "Door";
	bIsUsed = false;
}


void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	UIMessage += FString(ObjectName + " [Cost:" + FString::FromInt(Cost) + "]");
	SetReplicates(true);
}

// replicates bIsUsed to all clients/players in the game
void ABarricade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABarricade, bIsUsed);
}

void ABarricade::OnRep_BarricadeUsed()
{
	SetActorEnableCollision(false);
}

void ABarricade::Use(ACharacterBase* Player)
{
	// play animation to move barricade
	UE_LOG(LogTemp, Warning, TEXT("BARRICADE USE FUNCTION FOR %s"), *GetName());
	//SetActorEnableCollision(false);
	bIsUsed = true;
	OnRep_BarricadeUsed();
}
