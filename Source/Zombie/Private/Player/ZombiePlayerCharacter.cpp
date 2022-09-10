// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombiePlayerCharacter.h"
#include "Zombie/Useables/InteractableBase.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

AZombiePlayerCharacter::AZombiePlayerCharacter()
{
	Interactable = nullptr;
	InteractionRange = 250.0f;
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