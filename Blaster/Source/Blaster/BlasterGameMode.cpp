// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlasterGameMode.h"
#include "BlasterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABlasterGameMode::ABlasterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_BlasterCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
