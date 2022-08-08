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
#include "GameplayFramework/SATORI_GameInstance.h"

// Sets default values for this component's properties
USATORI_GameplayAbilityComponent::USATORI_GameplayAbilityComponent()
{

}

// Called when the game starts
void USATORI_GameplayAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ASATORICharacter>(GetOwner());
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

	//FirstAbilityValue = PlayerAbilitiesNames.Num() - 1;
	
	/*if (PlayerCharacter)
	{
		if (PortalRewardAbilities.Num() == 2)
		{
			FirstAbilityValue = PortalRewardAbilities.Num() - 1;
			LastAbilityValue = 1;
		}

		if (PortalRewardAbilities.Num() == 3)
		{
			FirstAbilityValue = PortalRewardAbilities.Num() - 1;
			LastAbilityValue = CurrentAbilityValue + 1;
		}
	}*/

	if (PortalRewardAbilities.Num() == 1)
	{
		FirstAbilityValue = 0;
	}

	if (PortalRewardAbilities.Num() == 2)
	{
		FirstAbilityValue = 0;
		MidAbilityValue = 1;
	}

	if (PortalRewardAbilities.Num() == 3)
	{
		FirstAbilityValue = 0;
		MidAbilityValue = 1;
		LastAbilityValue = 2;
	}

	NotifyAbilityChanged();
}

bool USATORI_GameplayAbilityComponent::TryChangeAbility()
{
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

void USATORI_GameplayAbilityComponent::SetNextAbility()
{
	// If we have more than 1 abilties
	/*if (PortalRewardAbilities.Num() >= 2)
	{
		CurrentAbilityValue++;
		if (CurrentAbilityValue >= PortalRewardAbilities.Num())
			CurrentAbilityValue = 0;

		LastAbilityValue++;
		if (LastAbilityValue >= PortalRewardAbilities.Num())
			LastAbilityValue = 0;

		FirstAbilityValue++;
		if (FirstAbilityValue >= PortalRewardAbilities.Num())
			FirstAbilityValue = 0;
	}
	else if (PortalRewardAbilities.Num() >= 3)
	{
		CurrentAbilityValue++;
		if (CurrentAbilityValue >= PortalRewardAbilities.Num())
			CurrentAbilityValue = 0;

		LastAbilityValue++;
		if (LastAbilityValue >= PortalRewardAbilities.Num())
			LastAbilityValue = 0;

		FirstAbilityValue++;
		if (FirstAbilityValue >= PortalRewardAbilities.Num())
			FirstAbilityValue = 0;

	}
	else
	{
		FirstAbilityValue = 0;
		LastAbilityValue = 0;
		CurrentAbilityValue = 0;
	}*/

	NotifyAbilityChanged();
}

void USATORI_GameplayAbilityComponent::SetPrevAbility()
{
	/*if (PortalRewardAbilities.Num() > 1)
	{
		CurrentAbilityValue--;
		if (CurrentAbilityValue < 0)
			CurrentAbilityValue = PortalRewardAbilities.Num() - 1;

		LastAbilityValue--;
		if (LastAbilityValue < 0)
			LastAbilityValue = PortalRewardAbilities.Num() - 1;

		FirstAbilityValue--;
		if (FirstAbilityValue < 0)
			FirstAbilityValue = PortalRewardAbilities.Num() - 1;
	}
	else
	{
		FirstAbilityValue = 0;
		LastAbilityValue = 0;
		CurrentAbilityValue = 0;
	}*/

	NotifyAbilityChanged();
}

TSubclassOf<USATORI_GameplayAbility> USATORI_GameplayAbilityComponent::GetCurrentSatoriAbility()
{
	if (PortalRewardAbilities.Num() > 0)
	{
		const FSATORI_AbilitiesDatas* AbilityData = &PortalRewardAbilities[CurrentAbilityValue];
		if (AbilityData)
		{
			CurrentGameplayAbility = AbilityData->CurrentAbility;
			if (CurrentGameplayAbility)
			{
				return CurrentGameplayAbility;
			}
		}

	}

	return nullptr;
}


void USATORI_GameplayAbilityComponent::AddPortalAbilities(FSATORI_AbilitiesDatas AbilityData)
{
	PortalRewardAbilities.Add(AbilityData);
	UE_LOG(LogTemp, Display, TEXT(" Player Normal Abilities Numb : [%d] "), PortalRewardAbilities.Num());
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
	FSATORI_AbilitiesIconsDatas AbilityIconToChange;

	if (PlayerCharacter)
	{
		CheckAbilitiesStatus(AbilityIconToChange);
	}

	/*const FSATORI_AbilitiesDatas* MidAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[CurrentAbilityValue]);
	const FSATORI_AbilitiesDatas* LastAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[LastAbilityValue]);
	const FSATORI_AbilitiesDatas* FirstAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[FirstAbilityValue]);*/

	/*if (!MidAbilityData && !LastAbilityData && !FirstAbilityData)
		return;*/

	//AbilityIconChange.Broadcast(*AbilityData);

	AllAbilityIconChange.Broadcast(AbilityIconToChange);
}

void USATORI_GameplayAbilityComponent::CheckAbilitiesStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas)
{
	if (PortalRewardAbilities.Num() > 0)
	{
		/*MidAbilityData = &PortalRewardAbilities[MidAbilityValue] ? &PortalRewardAbilities[MidAbilityValue] : nullptr;
		LastAbilityData = &PortalRewardAbilities[LastAbilityValue] ? &PortalRewardAbilities[LastAbilityValue] : nullptr;
		FirstAbilityData = &PortalRewardAbilities[FirstAbilityValue] ? &PortalRewardAbilities[FirstAbilityValue] : nullptr;*/

		if (PortalRewardAbilities.Num() == 1)
		{
			FirstAbilityData = &PortalRewardAbilities[FirstAbilityValue];
			AbilitiesDatas.CurrentAbilitiyIcon = EmptyAbilitiyIcon;
			AbilitiesDatas.NextAbilitiyIcon = EmptyAbilitiyIcon;
			AbilitiesDatas.PrevAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
		}
		else if (PortalRewardAbilities.Num() == 2)
		{
			MidAbilityData = &PortalRewardAbilities[MidAbilityValue];
			FirstAbilityData = &PortalRewardAbilities[FirstAbilityValue];
			AbilitiesDatas.CurrentAbilitiyIcon = MidAbilityData->AbilitiyIcon;
			AbilitiesDatas.NextAbilitiyIcon = EmptyAbilitiyIcon;
			AbilitiesDatas.PrevAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
		}
		else if (PortalRewardAbilities.Num() == 3)
		{
			MidAbilityData = &PortalRewardAbilities[MidAbilityValue];
			LastAbilityData = &PortalRewardAbilities[LastAbilityValue];
			FirstAbilityData = &PortalRewardAbilities[FirstAbilityValue];

			AbilitiesDatas.CurrentAbilitiyIcon = MidAbilityData->AbilitiyIcon;
			AbilitiesDatas.NextAbilitiyIcon = LastAbilityData->AbilitiyIcon;
			AbilitiesDatas.PrevAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
		}
		else
		{
			MidAbilityData = &PortalRewardAbilities[MidAbilityValue];
			LastAbilityData = &PortalRewardAbilities[LastAbilityValue];
			FirstAbilityData = &PortalRewardAbilities[FirstAbilityValue];

			AbilitiesDatas.CurrentAbilitiyIcon = MidAbilityData->AbilitiyIcon;
			AbilitiesDatas.NextAbilitiyIcon = LastAbilityData->AbilitiyIcon;
			AbilitiesDatas.PrevAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
		}
	}
	else if (PortalRewardAbilities.Num() == 0)
	{
		AbilitiesDatas.CurrentAbilitiyIcon = EmptyAbilitiyIcon;
		AbilitiesDatas.NextAbilitiyIcon = EmptyAbilitiyIcon;
		AbilitiesDatas.PrevAbilitiyIcon = EmptyAbilitiyIcon;
	}
}

void USATORI_GameplayAbilityComponent::SetSavedAbilitiesWithGameInstance(USATORI_GameInstance* GameInstance)
{
	PortalRewardAbilities = GameInstance->NormalAbilities;
}

void USATORI_GameplayAbilityComponent::ResetCurrentPlayerAbilities()
{
	PortalRewardAbilities.Empty();
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