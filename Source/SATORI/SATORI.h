// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

#define ACTOR_ROLE_FSTRING *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(GetLocalRole()))
#define GET_ACTOR_ROLE_FSTRING(Actor) *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(Actor->GetLocalRole()))

// Binding Input to the ASC
UENUM(BlueprintType)
namespace ESATORIAbilityInputID
{
	enum Type
	{
		// 0 None
		None			UMETA(DisplayName = "None"),
		// 1 Confirm
		Confirm			UMETA(DisplayName = "Confirm"),
		// 2 Cancel
		Cancel			UMETA(DisplayName = "Cancel"),
		// Dash
		Dash			UMETA(DisplayName = "Dash"),
		// Attack
		Attack			UMETA(DisplayName = "Attack"),
		// HeavyAttack
		HeavyAttack		UMETA(DisplayName = "HeavyAttack"),
		// Push
		Push			UMETA(DisplayName = "Push"),
		// Pull
		Pull			UMETA(DisplayName = "Pull"),
		// Misile
		Misile			UMETA(DisplayName = "Misile"),
		// BlackHole
		BlackHole		UMETA(DisplayName = "BlackHole"),
		// Decoy
		Decoy			UMETA(DisplayName = "Decoy"),
		// Clone
		Clone			UMETA(DisplayName = "Clone"),
		// TimeSlow
		TimeSlow		UMETA(DisplayName = "TimeSlow"),
		// TimeStop
		TimeStop		UMETA(DisplayName = "TimeStop")
	};
}