// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AbilityDataAsset.h"
#include "AbilitySystemComponent.h"

USATORI_AbilityDataAsset::USATORI_AbilityDataAsset()
{

}

void USATORI_AbilityDataAsset::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const
{
	for (const FSATORIGameplayAbilityInfo& BindInfo : Abilities)
	{
		if (BindInfo.SATORIAbility)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(BindInfo.SATORIAbility, 1, (int32)BindInfo.AbilityKeys, AbilitySystemComponent->GetOwner()));
		}
	}
}