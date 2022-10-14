// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "ZombiePlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, NewInteractMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, NewPoints);

UCLASS()
class ZOMBIE_API AZombiePlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	AZombiePlayerCharacter();

protected:
	UPROPERTY(BlueprintAssignable)
		FInteractChanged NewInteractMessage;

	UPROPERTY(BlueprintAssignable)
		FPointsChanged NewPoints;

	FTimerHandle TInteractTimerHandle;
	class AInteractableBase* Interactable;

	UPROPERTY(EditDefaultsOnly)
		float InteractionRange;

	UPROPERTY(EditDefaultsOnly) // set to replicate
		int32 Points; // can't make uint (unsigned int) b/c subtracting in DecrementPoints will never get negative and conditional is checking for negative values

protected:
	void SetInteractableObject();
	void Interact();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Interact(class AInteractableBase* InteractingObject);
	bool Server_Interact_Validate(class AInteractableBase* InteractingObject);
	void Server_Interact_Implementation(class AInteractableBase* InteractingObject);

	virtual void OnFire() override;

public:
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);

	UFUNCTION(BlueprintCallable)
		int32 GetPoints();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
