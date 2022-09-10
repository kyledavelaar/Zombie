// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameMode.h"
#include "ZombieHUD.h"
//#include "ZombieCharacter.h"
#include "Zombie/Public/Player/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"

AZombieGameMode::AZombieGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase_C"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AZombieHUD::StaticClass();
}
