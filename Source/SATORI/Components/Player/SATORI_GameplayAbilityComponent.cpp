// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "Engine/DataTable.h"
#include "Data/SATORI_DT_AbilitiesTypes.h"
#include "UI/SATORI_MainUI.h"
#include "UI/Abilities/SATORI_ChangeAbilitiesWidget.h"

// Sets default values for this component's properties
USATORI_GameplayAbilityComponent::USATORI_GameplayAbilityComponent()
{

}

// Called when the game starts
void USATORI_GameplayAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultAbilities)
	{
		for (FSATORIGameplayAbilityInfo Ability : DefaultAbilities->Abilities)
		{
			FString CurrentName = Ability.AbilityName.ToString();
			FName LocalAbilityName = FName(*CurrentName);

			if (LocalAbilityName.IsValid() && Ability.SATORIAbility.Get())
			{
				FSATORI_AbilitiesDatas abilitiesData;
				abilitiesData.CurrentAbilities = Ability.SATORIAbility;
				PlayerGameplayAbility.Add(LocalAbilityName, abilitiesData);
			}
		}
	}

	if (AbilitiesIconDatas)
	{
		for (const TPair<FName, FSATORI_AbilitiesDatas>& pair : PlayerGameplayAbility)
		{
			const FAbilitesIconDatas* ItemDetails =
				AbilitiesIconDatas->FindRow<FAbilitesIconDatas>(pair.Key, TEXT("USATORI_GameplayAbilityComponent::BeginPlay"));

			if (ItemDetails)
			{			
				ASATORICharacter* Character = Cast<ASATORICharacter>(GetOwner());	
				if (Character)
				{
					ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
					if (PlayerController)
					{
						USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
						if (MainUI)
						{
							MainUI->SetAbilityIcon(ItemDetails->Icon);
						}
					}
				}
			}
		}
	}
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

void USATORI_GameplayAbilityComponent::RemoveEnabledAbilityClass(TSubclassOf<USATORI_GameplayAbility> ClassToRemove)
{
	if (IsAbilityClassEnabled(ClassToRemove))
	{
		EnabledAbilityClasses.Remove(ClassToRemove);
	}
}

bool USATORI_GameplayAbilityComponent::IsAbilityClassEnabled(TSubclassOf<USATORI_GameplayAbility> ClassToCheck) const
{
	return EnabledAbilityClasses.Find(ClassToCheck) != INDEX_NONE;
}