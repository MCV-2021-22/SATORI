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
	
	// Cooldown Timer Info
	FAbilityCooldownTimerInfo LocalAbilityCD_Info;
	LocalAbilityCD_Info.AbilityID = 0;
	LocalAbilityCD_Info.TimeRemaining = 0.f;

	FSATORI_CooldownDatas LocalCooldownData;
	LocalCooldownData.TimeRemained = 0.0f;
	LocalCooldownData.IsCooldownAvaiable = false;
	
	AbilityCD_Infos.Init(LocalAbilityCD_Info, 3);
	M_CooldownDatas.Init(LocalCooldownData, 3);

	// Noity Changes
	NotifyAbilityChanged();

	// Testing
	if (IsAllHabilityTesting)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, PlayerAbilitiesNames[CurrentAbilityValue].ToString());
	}
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
	// Only test for abilities
	if (IsAllHabilityTesting)
	{
		CurrentAbilityValue++;
		if (CurrentAbilityValue >= PlayerGameplayAbility.Num())
			CurrentAbilityValue = 0;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, PlayerAbilitiesNames[CurrentAbilityValue].ToString());
	}
	else
	{
		// Check if we have abilities
		if (PortalRewardAbilities.Num() >= 1)
		{
			CurrentAbilityValue++;
			if (CurrentAbilityValue >= PortalRewardAbilities.Num())
				CurrentAbilityValue = 0;
		}
		else
		{
			CurrentAbilityValue = 0;
		}
	}

	if (CurrentAbilityValue < 0)
	{
		CurrentAbilityValue = 0;
	}

	UE_LOG(LogTemp, Display, TEXT(" Set Next Current Ability Value : [%d] "), CurrentAbilityValue);

	NotifyAbilityChanged();
}

void USATORI_GameplayAbilityComponent::SetPrevAbility()
{
	// Only test for abilities
	if (IsAllHabilityTesting)
	{
		CurrentAbilityValue--;
		if (CurrentAbilityValue < 0)
			CurrentAbilityValue = PlayerGameplayAbility.Num() - 1;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, PlayerAbilitiesNames[CurrentAbilityValue].ToString());
	}
	else
	{
		// Check if we have abilities
		if (PortalRewardAbilities.Num() >= 1)
		{
			CurrentAbilityValue--;
			if (CurrentAbilityValue < 0)
			{
				CurrentAbilityValue = PortalRewardAbilities.Num() - 1;
			}
		}
		else
		{
			CurrentAbilityValue = 0;
		}
	}

	if (CurrentAbilityValue < 0)
	{
		CurrentAbilityValue = 0;
	}

	UE_LOG(LogTemp, Display, TEXT(" Set Prev Current Ability Value : [%d] "), CurrentAbilityValue);

	NotifyAbilityChanged();
}

TSubclassOf<USATORI_GameplayAbility> USATORI_GameplayAbilityComponent::GetCurrentSatoriAbility()
{
	if (!IsAllHabilityTesting)
	{
		if (PortalRewardAbilities.Num() > 0 && CurrentAbilityValue < PortalRewardAbilities.Num())
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
		// Avoid for the time stop value out of the array index IDK if work
		else if ((CurrentAbilityValue > 3 || CurrentAbilityValue < 0) && PortalRewardAbilities.Num() > 0)
		{
			CurrentAbilityValue = 0;
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
		else
		{
			CurrentAbilityValue = 0;
			if(PortalRewardAbilities.Num() > 0 && CurrentAbilityValue < PortalRewardAbilities.Num())
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
			
		}
	}
	// Test for Ability 
	else if(IsAllHabilityTesting)
	{
		if (PlayerGameplayAbility.Num() > 0)
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
		}
	}
	return CurrentGameplayAbility.Get() ? CurrentGameplayAbility : nullptr;
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
	if (PlayerCharacter)
	{
		CheckAbilitiesStatus(AbilityIconToChange);
		CheckAbilitiesBorderStatus(AbilityIconToChange);
	}

	AllAbilityIconChange.Broadcast(AbilityIconToChange);
}

void USATORI_GameplayAbilityComponent::NotifyCooldownAbilityChanged(float TimeRemaining)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Hability Value : %d"), CurrentAbilityValue));
	if (!M_CooldownDatas[CurrentAbilityValue].IsCooldownAvaiable)
	{
		M_CooldownDatas[CurrentAbilityValue].IsCooldownAvaiable = true;
		M_CooldownDatas[CurrentAbilityValue].AbilityID = CurrentAbilityValue;
		M_CooldownDatas[CurrentAbilityValue].TimeRemained = TimeRemaining;
		CooldownAbilityIconChange.Broadcast(M_CooldownDatas[CurrentAbilityValue]);

		// Fill the ability CD Info
		AbilityCD_Infos[CurrentAbilityValue].AbilityID = CurrentAbilityValue;
		AbilityCD_Infos[CurrentAbilityValue].TimeRemaining = TimeRemaining;

		CheckCooldownTimeRemaines(AbilityCD_Infos[CurrentAbilityValue].AbilityID);
	}
}

void USATORI_GameplayAbilityComponent::CheckAbilitiesStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas)
{
	if (PortalRewardAbilities.Num() > 0)
	{
		if (PortalRewardAbilities.Num() == 1)
		{
			FirstAbilityData = &PortalRewardAbilities[0];
			// Fill the datas
			AbilitiesDatas.FirstAbilitiyIcon = FirstAbilityData->AbilitiyIcon;;
			AbilitiesDatas.SecondAbilitiyIcon = EmptyAbilitiyIcon;
			AbilitiesDatas.LastAbilitiyIcon = EmptyAbilitiyIcon;
		}
		else if (PortalRewardAbilities.Num() == 2)
		{
			FirstAbilityData = &PortalRewardAbilities[0];
			SecondAbilityData = &PortalRewardAbilities[1];
			// Fill the datas
			AbilitiesDatas.FirstAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
			AbilitiesDatas.SecondAbilitiyIcon = SecondAbilityData->AbilitiyIcon;
			AbilitiesDatas.LastAbilitiyIcon = EmptyAbilitiyIcon;
		}
		else if (PortalRewardAbilities.Num() == 3)
		{
			FirstAbilityData = &PortalRewardAbilities[0];
			SecondAbilityData = &PortalRewardAbilities[1];
			LastAbilityData = &PortalRewardAbilities[2];
			// Fill the datas
			AbilitiesDatas.FirstAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
			AbilitiesDatas.SecondAbilitiyIcon = SecondAbilityData->AbilitiyIcon;
			AbilitiesDatas.LastAbilitiyIcon = LastAbilityData->AbilitiyIcon;
		}
		else
		{
			FirstAbilityData = &PortalRewardAbilities[0];
			SecondAbilityData = &PortalRewardAbilities[1];
			LastAbilityData = &PortalRewardAbilities[2];
			// Fill the datas
			AbilitiesDatas.FirstAbilitiyIcon = FirstAbilityData->AbilitiyIcon;
			AbilitiesDatas.SecondAbilitiyIcon = SecondAbilityData->AbilitiyIcon;
			AbilitiesDatas.LastAbilitiyIcon = LastAbilityData->AbilitiyIcon;
		}
	}
	// All empty Icon
	else if (PortalRewardAbilities.Num() == 0)
	{
		AbilitiesDatas.FirstAbilitiyIcon = EmptyAbilitiyIcon;
		AbilitiesDatas.SecondAbilitiyIcon = EmptyAbilitiyIcon;
		AbilitiesDatas.LastAbilitiyIcon = EmptyAbilitiyIcon;
	}
}

void USATORI_GameplayAbilityComponent::CheckAbilitiesBorderStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas)
{
	if (PortalRewardAbilities.Num() <= 0)
	{
		AbilitiesDatas.AbilitiesBordesChecker.IsFirstIconAvaiable = false;
		AbilitiesDatas.AbilitiesBordesChecker.IsSecondIconAvaiable = false;
		AbilitiesDatas.AbilitiesBordesChecker.IsThirstIconAvaiable = false;
	}
	else if (CurrentAbilityValue == 0 && PortalRewardAbilities.Num() > 0)
	{
		AbilitiesDatas.AbilitiesBordesChecker.IsFirstIconAvaiable = true;
		AbilitiesDatas.AbilitiesBordesChecker.IsSecondIconAvaiable = false;
		AbilitiesDatas.AbilitiesBordesChecker.IsThirstIconAvaiable = false;
	}
	else if (CurrentAbilityValue == 1 && PortalRewardAbilities.Num() > 0)
	{
		AbilitiesDatas.AbilitiesBordesChecker.IsFirstIconAvaiable = false;
		AbilitiesDatas.AbilitiesBordesChecker.IsSecondIconAvaiable = true;
		AbilitiesDatas.AbilitiesBordesChecker.IsThirstIconAvaiable = false;
	}
	else if (CurrentAbilityValue == 2 && PortalRewardAbilities.Num() > 0)
	{
		AbilitiesDatas.AbilitiesBordesChecker.IsFirstIconAvaiable = false;
		AbilitiesDatas.AbilitiesBordesChecker.IsSecondIconAvaiable = false;
		AbilitiesDatas.AbilitiesBordesChecker.IsThirstIconAvaiable = true;
	}
}

void USATORI_GameplayAbilityComponent::SetSavedAbilitiesWithGameInstance(USATORI_GameInstance* GameInstance)
{
	PortalRewardAbilities = GameInstance->NormalAbilities;
	CurrentAbilityValue = GameInstance->CurrentPlayerAbilityId;
}

void USATORI_GameplayAbilityComponent::ResetCurrentPlayerAbilities()
{
	PortalRewardAbilities.Empty();
}

void USATORI_GameplayAbilityComponent::CheckCooldownTimeRemaines(int HabilityID)
{
	/*FTimerDelegate CooldownDelegate;*/

	AbilityCD_Infos[HabilityID].CooldownDelegate = FTimerDelegate::CreateUObject(this, 
		&USATORI_GameplayAbilityComponent::CooldownCountDown, HabilityID);

	GetWorld()->GetTimerManager().SetTimer(AbilityCD_Infos[HabilityID].WaitHandle, 
		AbilityCD_Infos[HabilityID].CooldownDelegate, 1, true, 0.0);
}

void USATORI_GameplayAbilityComponent::CooldownCountDown(int HabilityID)
{
	AbilityCD_Infos[HabilityID].TimeRemaining -= 1.0f;
	if (AbilityCD_Infos[HabilityID].TimeRemaining <= 0)
	{
		M_CooldownDatas[HabilityID].IsCooldownAvaiable = false;
		M_CooldownDatas[HabilityID].TimeRemained = 0.0f;
		M_CooldownDatas[HabilityID].AbilityID = 0;
		CooldownAbilityIconChange.Broadcast(M_CooldownDatas[HabilityID]);

		// Time Handle clean
		AbilityCD_Infos[HabilityID].AbilityID = 0;
		AbilityCD_Infos[HabilityID].TimeRemaining = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(AbilityCD_Infos[HabilityID].WaitHandle);
	}
}