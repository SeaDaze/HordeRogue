// Copyright Epic Games, Inc. All Rights Reserved.

#include "HordeGameGameMode.h"
#include "HordeGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHordeGameGameMode::AHordeGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
