// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_GameGameMode.h"
#include "RPG_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPG_GameGameMode::ARPG_GameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
