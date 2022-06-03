// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "Data/SATORI_ChooseAbilitiesDatas.h"
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

	if (ChoosesAbilities)
	{
		for (FAbilitiesChooseDatas Ability : ChoosesAbilities->Abilities)
		{
			FString CurrentName = Ability.AbilityName.ToString();
			FName LocalAbilityName = FName(*CurrentName);

			if (LocalAbilityName.IsValid() && Ability.SATORIAbility.Get())
			{
				// Adding datas to map
				FSATORI_AbilitiesDatas abilitiesData;
				abilitiesData.CurrentAbility = Ability.SATORIAbility;
				abilitiesData.AbilitiyIcon = Ability.AbilitiyIcon;
				abilitiesData.AbilityName = Ability.AbilityName;
				PlayerGameplayAbility.Add(LocalAbilityName, abilitiesData);

				// Array abilties names
				PlayerAbilitiesNames.Add(LocalAbilityName);
			}
		}
	}

	//UpdateAbilityIcon();
	/*if (AbilitiesIconDatas)
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
	}*/

	
	NotifyAbilityChanged();
	
	//FName CurrentName = PlayerAbilitiesNames[CurrentAbilityValue];
	//FSATORI_AbilitiesDatas* LocalAbilityData = PlayerGameplayAbility.Find(CurrentName);

	//if (LocalAbilityData)
	//{
	//	//AbilityIconChange.Broadcast(LocalAbilityData);

	//	ASATORICharacter* Character = Cast<ASATORICharacter>(GetOwner());
	//	if (Character)
	//	{
	//		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
	//		if (PlayerController)
	//		{

	//			USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
	//			if (MainUI)
	//			{
	//				MainUI->SetAbilityIcon(LocalAbilityData->AbilitiyIcon);
	//			}
	//		}
	//	}
	//}
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

//FSATORI_AbilitiesDatas* USATORI_GameplayAbilityComponent::SetNextAbility()
//{
//	FName CurrentAbilityName = PlayerAbilitiesNames[CurrentAbilityValue];
//	FSATORI_AbilitiesDatas* CurrentAbilitiesDatas = PlayerGameplayAbility.Find(CurrentAbilityName);
//	if(CurrentAbilitiesDatas)
//		return CurrentAbilitiesDatas;
//
//	return nullptr;
//}

//void USATORI_GameplayAbilityComponent::UpdateAbilityIcon()
//{
//	const USATORI_GameplayAbility* CurrentAbilityInfo = Cast<USATORI_GameplayAbility>(EnabledAbilityClasses[CurrentAbilityValue].Get());
//	if (CurrentAbilityInfo)
//	{
//		ASATORICharacter* Character = Cast<ASATORICharacter>(GetOwner());
//		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());
//		if (PlayerController)
//		{
//			USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
//			if (MainUI)
//			{
//				MainUI->SetAbilityIcon(CurrentAbilityInfo->AbilityIcon);
//			}
//		}
//	}
//}

TSubclassOf<USATORI_GameplayAbility> USATORI_GameplayAbilityComponent::GetCurrentAbility()
{
	FName CurrentName = PlayerAbilitiesNames[CurrentAbilityValue];
	if (CurrentName.IsValid())
	{
		FSATORI_AbilitiesDatas* LocalAbilityData = PlayerGameplayAbility.Find(CurrentName);
		CurrentGameplayAbility = LocalAbilityData->CurrentAbility;
		if(CurrentGameplayAbility)
		{
			return CurrentGameplayAbility;
		}
	}
	
	FSATORI_AbilitiesDatas* FirstAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[CurrentAbilityValue]);
	CurrentGameplayAbility = FirstAbilityData->CurrentAbility;
	return CurrentGameplayAbility;
}

void USATORI_GameplayAbilityComponent::SetNextAbility()
{
	CurrentAbilityValue++;
	if (CurrentAbilityValue >= EnabledAbilityClasses.Num())
		CurrentAbilityValue = 0;
	NotifyAbilityChanged();
}

TSubclassOf<USATORI_GameplayAbility> USATORI_GameplayAbilityComponent::GetCurrentSatoriAbility()
{
	const FSATORI_AbilitiesDatas* AbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[CurrentAbilityValue]);
	if (AbilityData)
	{
		CurrentGameplayAbility = AbilityData->CurrentAbility;
		if (CurrentGameplayAbility)
		{
			return CurrentGameplayAbility;
		}
	}

	return AbilityData->CurrentAbility;
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

void USATORI_GameplayAbilityComponent::NotifyAbilityChanged()
{
	const FSATORI_AbilitiesDatas* AbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[CurrentAbilityValue]);
	if (!AbilityData)
		return;

	AbilityIconChange.Broadcast(*AbilityData);
}