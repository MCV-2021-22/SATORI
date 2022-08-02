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

	//PrevAbilityValue = PlayerAbilitiesNames.Num() - 1;
	
	if (PlayerCharacter && !PlayerCharacter->GetIsAbilityUpgrated())
	{
		if (NormalAbilities.Num() < 0)
		{
			PrevAbilityValue = 0;
			NextAbilityValue = 0;
		}
		else if (NormalAbilities.Num() == 0)
		{
			PrevAbilityValue = UpgratedAbilities.Num();
			NextAbilityValue = 0;
		}
	}
	else if (PlayerCharacter && PlayerCharacter->GetIsAbilityUpgrated())
	{
		if (UpgratedAbilities.Num() < 0)
		{
			PrevAbilityValue = 0;
			NextAbilityValue = 0;
		}
		else if (UpgratedAbilities.Num() == 0)
		{
			PrevAbilityValue = UpgratedAbilities.Num();
			NextAbilityValue = 0;
		}
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


void USATORI_GameplayAbilityComponent::AddNormalAbilities(FSATORI_AbilitiesDatas AbilityData)
{
	NormalAbilities.Add(AbilityData);
	UE_LOG(LogTemp, Display, TEXT(" Player Normal Abilities Numb : [%d] "), NormalAbilities.Num());
}

void USATORI_GameplayAbilityComponent::AddUpgratedAbilities(FSATORI_AbilitiesDatas AbilityData)
{
	UpgratedAbilities.Add(AbilityData);
	UE_LOG(LogTemp, Display, TEXT(" Player Upgrated Abilities Numb : [%d] "), UpgratedAbilities.Num());
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
	const FSATORI_AbilitiesDatas* CurrentAbilityData = nullptr;
	const FSATORI_AbilitiesDatas* NextAbilityData = nullptr;
	const FSATORI_AbilitiesDatas* PrevAbilityData = nullptr;

	FSATORI_AbilitiesIconsDatas AbilityIconToChange;

	// Check if it is normal abilities
	if (PlayerCharacter && !PlayerCharacter->GetIsAbilityUpgrated())
	{
		if (NormalAbilities.Num() > 0)
		{
			CurrentAbilityData = &NormalAbilities[CurrentAbilityValue];
			NextAbilityData = &NormalAbilities[NextAbilityValue];
			PrevAbilityData = &NormalAbilities[PrevAbilityValue];
			
			if (NormalAbilities.Num() == 1)
			{
				AbilityIconToChange.CurrentAbilitiyIcon = CurrentAbilityData->AbilitiyIcon;
				AbilityIconToChange.NextAbilitiyIcon = NextAbilityData->AbilitiyIcon;
				AbilityIconToChange.PrevAbilitiyIcon = PrevAbilityData->AbilitiyIcon;
			}
		}
		else if (NormalAbilities.Num() == 0)
		{
			AbilityIconToChange.CurrentAbilitiyIcon = EmptyAbilitiyIcon;
			AbilityIconToChange.NextAbilitiyIcon = EmptyAbilitiyIcon;
			AbilityIconToChange.PrevAbilitiyIcon = EmptyAbilitiyIcon;
		}
	}
	// Check if it is upgrated abilities
	else
	{
		CurrentAbilityData = &UpgratedAbilities[CurrentAbilityValue];
		NextAbilityData = &UpgratedAbilities[NextAbilityValue];
		PrevAbilityData = &UpgratedAbilities[PrevAbilityValue];
	}

	/*const FSATORI_AbilitiesDatas* CurrentAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[CurrentAbilityValue]);
	const FSATORI_AbilitiesDatas* NextAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[NextAbilityValue]);
	const FSATORI_AbilitiesDatas* PrevAbilityData = PlayerGameplayAbility.Find(PlayerAbilitiesNames[PrevAbilityValue]);*/

	/*if (!CurrentAbilityData && !NextAbilityData && !PrevAbilityData)
		return;*/

	//AbilityIconChange.Broadcast(*AbilityData);

	AllAbilityIconChange.Broadcast(AbilityIconToChange);
}

void USATORI_GameplayAbilityComponent::SetSavedAbilitiesWithGameInstance(USATORI_GameInstance* GameInstance)
{
	NormalAbilities = GameInstance->NormalAbilities;
	UpgratedAbilities = GameInstance->UpgratedAbilities;
}

void USATORI_GameplayAbilityComponent::ResetCurrentPlayerAbilities()
{
	NormalAbilities.Empty(); 
	UpgratedAbilities.Empty(); 
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