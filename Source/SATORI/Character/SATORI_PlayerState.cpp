// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_PlayerState.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"

ASATORI_PlayerState::ASATORI_PlayerState()
{
	AttributeSetBase = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSetBase"));
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

void ASATORI_PlayerState::BeginPlay()
{

}
