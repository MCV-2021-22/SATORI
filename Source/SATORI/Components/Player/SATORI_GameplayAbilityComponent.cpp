// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "SATORICharacter.h"

// Sets default values for this component's properties
USATORI_GameplayAbilityComponent::USATORI_GameplayAbilityComponent()
{

}

// Called when the game starts
void USATORI_GameplayAbilityComponent::BeginPlay()
{
	Super::BeginPlay();


	/*if (DefaultAbilities)
	{
		for (FSATORIGameplayAbilityInfo Ability : DefaultAbilities->Abilities)
		{
			FString CurrentName = Ability.AbilityName.ToString();
			FName LocalAbilityName = FName(*CurrentName);

			if (LocalAbilityName.IsValid() && Ability.SATORIAbility.Get())
			{
				PlayerGameplayAbility.Add(LocalAbilityName, Ability.SATORIAbility);
			}
		}
	}*/
}

bool USATORI_GameplayAbilityComponent::TryChangeAbility()
{
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(GetOwner());
	if (PlayerCharacter)
	{
		FGameplayTag TagToActivate;
		TagToActivate.EmptyTag;
		if (PlayerCharacter->PlayerActiveAbilityWithTag(TagToActivate))
		{
			return true;
		}
	}
	return false;
}

void USATORI_GameplayAbilityComponent::AddEnabledAbilityClass(TSubclassOf<USATORI_GameplayAbility> ClassToAdd)
{
	EnabledAbilityClasses.AddUnique(ClassToAdd);
}

bool USATORI_GameplayAbilityComponent::IsAbilityClassEnabled(TSubclassOf<USATORI_GameplayAbility> ClassToCheck) const
{
	return EnabledAbilityClasses.Find(ClassToCheck) != INDEX_NONE;
}