// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "SATORICharacter.h"

// Sets default values for this component's properties
USATORI_ComboSystemComponent::USATORI_ComboSystemComponent()
{
	//isLightAttack = true;
	//isHeavyAttack = true;
}


// Called when the game starts
void USATORI_ComboSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USATORI_ComboSystemComponent::LigthAttack()
{
	if (isLightAttack)
	{
		isHeavyAttack = false;
		//isLightAttack = false;
		if (lightAttackCounter >= LightAttackMontageToPlay.Num() - 1)
		{
			lightAttackCounter = 0;
		}
		else
		{
			lightAttackCounter ++;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("lightAttackCounter : %d"), lightAttackCounter);
}

void USATORI_ComboSystemComponent::HeavyAttack()
{
	/*if (isHeavyAttack)
	{
		isLightAttack = false;
	}*/
	
}

void USATORI_ComboSystemComponent::ResetAllAttribute()
{
	/*isLightAttack = true;
	isHeavyAttack = true;*/
	lightAttackCounter = 0;
	HeavyAttackCounter = 0;
}

bool USATORI_ComboSystemComponent::CanComboAttack()
{
	if (isLightAttack || isHeavyAttack)
		return true;

	return false;
}