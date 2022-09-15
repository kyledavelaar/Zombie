// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombiePlayerCharacter.h"
#include "Zombie/ZombieCharacter/ZombieBase.h"
#include "Zombie/Useables/InteractableBase.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

AZombiePlayerCharacter::AZombiePlayerCharacter()
{
	Interactable = nullptr;
	InteractionRange = 250.0f;
	Points = 500;
}

void AZombiePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &AZombiePlayerCharacter::SetInteractableObject, 0.2f, true);
}

void AZombiePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AZombiePlayerCharacter::Interact);
}

void AZombiePlayerCharacter::SetInteractableObject()
{
	// LineTrace 
	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector Rotation = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	FVector End = Start + Rotation * InteractionRange;
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams);

	AInteractableBase* Temp = Cast<AInteractableBase>(HitResult.GetActor());
	if (Interactable == nullptr && Temp)
	{
		// started looking at interactable for the first time
		UE_LOG(LogTemp, Warning, TEXT("started looking at interactable for the first time: %s"), *HitResult.GetActor()->GetName());
		Interactable = Temp;
		OnInteractChanged.Broadcast(Interactable->GetUIMessage());
	}
	else if (Interactable && Temp == nullptr)
	{
		// stopped looking at interactable
		UE_LOG(LogTemp, Warning, TEXT("stopped looking at interactable"));
		Interactable = nullptr;
		OnInteractChanged.Broadcast(FString());
	}
}

void AZombiePlayerCharacter::Interact()
{
	// perform server RPC if client
	if (Interactable)
	{
		if (HasAuthority()) // is server (host)
		{
			UE_LOG(LogTemp, Warning, TEXT("HAS AUTHORITY"));
			Interactable->Use(this);
		}
		else // is client
		{
			UE_LOG(LogTemp, Warning, TEXT("DOES NOT HAVE AUTHORITY"));
			// makes call to server
			Server_Interact(Interactable);
		}
	}
}

bool AZombiePlayerCharacter::Server_Interact_Validate(AInteractableBase* InteractingObject)
{
	return true;
}

void AZombiePlayerCharacter::Server_Interact_Implementation(AInteractableBase* InteractingObject)
{
	float Distance = GetDistanceTo(InteractingObject);
	if (Distance < InteractionRange + 50.0f)
	{
		InteractingObject->Use(this);
	}
}

void AZombiePlayerCharacter::OnFire()
{
	// line trace setup to hit zombie
	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	FVector End = Start + Rot * 2000.0f;
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams))
	{
		if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT, %s"), *Zombie->GetName());
			Zombie->Hit(this);
		}
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

}

void AZombiePlayerCharacter::IncrementPoints(uint16 Value)
{
	Points += Value;
	UE_LOG(LogTemp, Warning, TEXT("POINTS: %d"), Points);
}

bool AZombiePlayerCharacter::DecrementPoints(uint16 Value)
{
	if ((Points - Value) < 0)
	{
		return false;
	}
	else
	{
		Points -= Value;
		UE_LOG(LogTemp, Warning, TEXT("POINTS: %d"), Points);
		return true;
	}
}
