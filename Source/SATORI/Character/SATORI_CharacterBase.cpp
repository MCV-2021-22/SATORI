// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SATORI_CharacterBase.h"
#include "GAS/SATORI_AbilitySystemComponent.h"

// Sets default values
ASATORI_CharacterBase::ASATORI_CharacterBase()
{

}

// Called when the game starts or when spawned
void ASATORI_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ASATORI_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

