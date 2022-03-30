// Copyright Epic Games, Inc. All Rights Reserved.

#include "SATORIGameMode.h"
#include "SATORICharacter.h"
#include "UObject/ConstructorHelpers.h"

ASATORIGameMode::ASATORIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SATORI/Character/SATORI_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("/Game/SATORI/Character/SATORI_Character"));
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
