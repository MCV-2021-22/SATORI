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
		// ChooseAbility
		ChooseAbility	UMETA(DisplayName = "ChooseAbility"),
		// LaunchAbility
		LaunchAbility	UMETA(DisplayName = "LaunchAbility"),
		// ChoosePrevAbility
		ChoosePrevAbility	UMETA(DisplayName = "ChoosePrevAbility")
	};
}

UENUM(BlueprintType)
enum class ESATORI_AbilityInputID : uint8
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
	// ChooseAbility
	ChooseAbility	UMETA(DisplayName = "ChooseAbility"),
	// LaunchAbility
	LaunchAbility	UMETA(DisplayName = "LaunchAbility"),
	// ChoosePrevAbility
	ChoosePrevAbility	UMETA(DisplayName = "ChoosePrevAbility")
};