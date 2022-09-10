// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/InteractableBase.h"
#include "Zombie/Public/Player/CharacterBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	ObjectName = "Default";
	UIMessage = "Press F to buy ";
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FString AInteractableBase::GetUIMessage()
{
	return UIMessage;
}

void AInteractableBase::Use(ACharacterBase* Player)
{
}


