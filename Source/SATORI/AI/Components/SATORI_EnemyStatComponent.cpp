// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Components/SATORI_EnemyStatComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

USATORI_EnemyStatComponent::USATORI_EnemyStatComponent()
{

}

void USATORI_EnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SelfCharacter = Cast<ASATORI_AICharacter>(GetOwner());
	if (SelfCharacter)
	{
		InitializeStatsAttributes(SelfCharacter);
		BindAttributeChage(SelfCharacter);
	}

	UpdateHealthBarPercent.Broadcast(Health, MaxHealth);
}

void USATORI_EnemyStatComponent::InitializeStatsAttributes(ASATORI_AICharacter* Character)
{
	SATORIAttributes = Character->AttributeSet;
	if (SATORIAttributes.IsValid())
	{
		MaxHealth = SATORIAttributes->GetMaxHealth();
		Health = MaxHealth;
	}
}

void USATORI_EnemyStatComponent::BindAttributeChage(ASATORI_AICharacter* Character)
{
	AbilitySystemComponent = Character->GetAbilitySystemComponent();

	if (AbilitySystemComponent.IsValid())
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(SATORIAttributes->GetHealthAttribute()).AddUObject(this, &USATORI_EnemyStatComponent::HealthChanged);

		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate
		(SATORIAttributes->GetMaxHealthAttribute()).AddUObject(this, &USATORI_EnemyStatComponent::MaxHealthChanged);
	}
}

void USATORI_EnemyStatComponent::HealthChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	Health = NewValue;

	UpdateHealthBarPercent.Broadcast(Health, MaxHealth);
}

void USATORI_EnemyStatComponent::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float NewValue = Data.NewValue;
	float OldValue = Data.OldValue;

	MaxHealth = NewValue;

	UpdateHealthBarPercent.Broadcast(Health, MaxHealth);
}

float USATORI_EnemyStatComponent::GetHealth()
{
	return Health;
}

float USATORI_EnemyStatComponent::GetMaxHealth()
{
	return MaxHealth;
}