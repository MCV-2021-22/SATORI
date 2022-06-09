// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/SATORI_EnemyHealthBar.h"
#include "SATORICharacter.h"
#include "Character/SATORI_CharacterBase.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

bool USATORI_EnemyHealthBar::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void ShowHealthBar()
{
	
}