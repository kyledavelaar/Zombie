// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class ZOMBIE_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		FString UIMessage;

	UPROPERTY(EditAnywhere, Category = "ZombieSettings")
		FString ObjectName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	FString GetUIMessage();
	virtual void Use(class AZombiePlayerCharacter* Player);
};
