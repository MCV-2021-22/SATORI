// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_PlayerState.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"

ASATORI_PlayerState::ASATORI_PlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSetBase = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSetBase"));
}

UAbilitySystemComponent* ASATORI_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USATORI_AttributeSet* ASATORI_PlayerState::GetSatoriAttributeSet() const
{
	return AttributeSetBase;
}

float ASATORI_PlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ASATORI_PlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ASATORI_PlayerState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float ASATORI_PlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

float ASATORI_PlayerState::GetDefense() const
{
	return AttributeSetBase->GetDefense();
}

float ASATORI_PlayerState::GetAttack() const
{
	return AttributeSetBase->GetAttack();
}

float ASATORI_PlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

float ASATORI_PlayerState::GetGold() const
{
	return AttributeSetBase->GetGold();
}

void ASATORI_PlayerState::BeginPlay()
{
	Super::BeginPlay();

}
