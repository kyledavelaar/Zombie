// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/Barricade.h"
#include "Zombie/Game/ZombieGameMode2.h"
#include "Zombie/Public/Player/ZombiePlayerCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABarricade::ABarricade()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	Cost = 500;
	AccessZone = 0;
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
	//SetActorEnableCollision(false);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (OpenAnimation)
	{
		MeshComp->PlayAnimation(OpenAnimation, false);
		UIMessage = FString();
	}
}

void ABarricade::Use(AZombiePlayerCharacter* Player)
{
	if (HasAuthority() && !bIsUsed && Player && Player->DecrementPoints(Cost))
	{
		// play animation to move barricade
		UE_LOG(LogTemp, Warning, TEXT("BARRICADE USE FUNCTION FOR %s"), *GetName());
		//SetActorEnableCollision(false);
		bIsUsed = true;
		OnRep_BarricadeUsed();
		if (AZombieGameMode2* GM = GetWorld()->GetAuthGameMode<AZombieGameMode2>())
		{
			GM->NewZoneActive(AccessZone);
		}
	}
}

uint8 ABarricade::GetAccessZone()
{
	return AccessZone;
}
