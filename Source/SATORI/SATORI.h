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
		// 3 LMB
		Ability1		UMETA(DisplayName = "Ability1"),
		// 4 RMB
		Ability2		UMETA(DisplayName = "Ability2"),
		// 5 Q
		Ability3		UMETA(DisplayName = "Ability3"),
		// 6 E
		Ability4		UMETA(DisplayName = "Ability4"),
		// 7 R
		Ability5		UMETA(DisplayName = "Ability5"),
		// 8 Sprint
		Sprint			UMETA(DisplayName = "Sprint"),
		// 9 Jump
		Jump			UMETA(DisplayName = "Jump"),
		// Attack
		Attack			UMETA(DisplayName = "Attack"),
		// HeavyAttack
		HeavyAttack			UMETA(DisplayName = "HeavyAttack")
	};
}