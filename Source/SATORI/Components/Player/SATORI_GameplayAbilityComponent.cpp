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

	PrevAbilityValue = PlayerAbilitiesNames.Num() - 1;
	
	NotifyAbilityChanged();
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
	
	FSATORI_AbilitiesDatas* FirstAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[0]);
	CurrentGameplayAbility = FirstAbilityData->CurrentAbility;
	return CurrentGameplayAbility;
}

void USATORI_GameplayAbilityComponent::SetNextAbility()
{
	CurrentAbilityValue++;
	if (CurrentAbilityValue >= PlayerAbilitiesNames.Num())
		CurrentAbilityValue = 0;

	NextAbilityValue++;
	if (NextAbilityValue >= PlayerAbilitiesNames.Num())
		NextAbilityValue = 0;

	PrevAbilityValue++;
	if (PrevAbilityValue >= PlayerAbilitiesNames.Num())
		PrevAbilityValue = 0;

	NotifyAbilityChanged();
}

void USATORI_GameplayAbilityComponent::SetPrevAbility()
{
	CurrentAbilityValue--;
	if (CurrentAbilityValue < 0)
		CurrentAbilityValue = PlayerAbilitiesNames.Num() - 1;

	NextAbilityValue--;
	if (NextAbilityValue < 0)
		NextAbilityValue = PlayerAbilitiesNames.Num() - 1;

	PrevAbilityValue--;
	if (PrevAbilityValue < 0)
		PrevAbilityValue = PlayerAbilitiesNames.Num() - 1;

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


void USATORI_GameplayAbilityComponent::AddEnabledAbility()
{

}

void USATORI_GameplayAbilityComponent::RemoveEnabledAbility()
{

}

bool USATORI_GameplayAbilityComponent::IsAbilityEnabled() const
{
	return true;
}

void USATORI_GameplayAbilityComponent::NotifyAbilityChanged()
{
	const FSATORI_AbilitiesDatas* CurrentAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[CurrentAbilityValue]);

	const FSATORI_AbilitiesDatas* NextAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[NextAbilityValue]);

	const FSATORI_AbilitiesDatas* PrevAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[PrevAbilityValue]);

	if (!CurrentAbilityData && !NextAbilityData && !PrevAbilityData)
		return;

	FSATORI_AbilitiesIconsDatas AbilityIconToChange;
	AbilityIconToChange.CurrentAbilitiyIcon = CurrentAbilityData->AbilitiyIcon;
	AbilityIconToChange.NextAbilitiyIcon = NextAbilityData->AbilitiyIcon;
	AbilityIconToChange.PrevAbilitiyIcon = PrevAbilityData->AbilitiyIcon;

	//AbilityIconChange.Broadcast(*AbilityData);

	AllAbilityIconChange.Broadcast(AbilityIconToChange);
}

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