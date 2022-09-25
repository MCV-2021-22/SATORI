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
	
	NotifyAbilityChanged();

	if (IsNachoTesting)
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
	if (IsNachoTesting)
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
	UE_LOG(LogTemp, Display, TEXT(" Set Next Current Ability Value : [%d] "), CurrentAbilityValue);

	NotifyAbilityChanged();
}

void USATORI_GameplayAbilityComponent::SetPrevAbility()
{
	// Only test for abilities
	if (IsNachoTesting)
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
				CurrentAbilityValue = PortalRewardAbilities.Num() - 1;
		}
		else
		{
			CurrentAbilityValue = 0;
		}
	}

	UE_LOG(LogTemp, Display, TEXT(" Set Prev Current Ability Value : [%d] "), CurrentAbilityValue);

	NotifyAbilityChanged();
}

TSubclassOf<USATORI_GameplayAbility> USATORI_GameplayAbilityComponent::GetCurrentSatoriAbility()
{
	if (!IsNachoTesting)
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
	}
	// Test for Ability 
	else
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
	if (PlayerCharacter)
	{
		CheckAbilitiesStatus(AbilityIconToChange);
		CheckAbilitiesBorderStatus(AbilityIconToChange);
	}

	AllAbilityIconChange.Broadcast(AbilityIconToChange);
}

void USATORI_GameplayAbilityComponent::NotifyCooldownAbilityChanged(float TimeRemaining)
{
	//UE_LOG(LogTemp, Warning, TEXT("Time Remain, %f"), TimeRemaining);
	FSATORI_AbilitiesIconsCooldownDatas CooldownData;

	if (CurrentAbilityValue == 0 && !CooldownData.IsFirstCooldownAvaiable)
	{
		CooldownData.IsFirstCooldownAvaiable = true;
		CooldownAbilityIconChange.Broadcast(CooldownData);
		CheckCooldownTimeRemaines(TimeRemaining);
	}

	if (CurrentAbilityValue == 1 && !CooldownData.IsSecondCooldownAvaiable)
	{
		CooldownData.IsSecondCooldownAvaiable = true;
		CooldownAbilityIconChange.Broadcast(CooldownData);
		CheckCooldownTimeRemaines(TimeRemaining);
	}

	if (CurrentAbilityValue == 2 && !CooldownData.IsThirstCooldownAvaiable)
	{
		CooldownData.IsThirstCooldownAvaiable = true;
		CooldownAbilityIconChange.Broadcast(CooldownData);
		CheckCooldownTimeRemaines(TimeRemaining);
	}
	
}

void USATORI_GameplayAbilityComponent::CheckAbilitiesStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas)
{
	if (PortalRewardAbilities.Num() > 0)
	{
		/*SecondAbilityData = &PortalRewardAbilities[MidAbilityValue] ? &PortalRewardAbilities[MidAbilityValue] : nullptr;
		LastAbilityData = &PortalRewardAbilities[LastAbilityValue] ? &PortalRewardAbilities[LastAbilityValue] : nullptr;
		FirstAbilityData = &PortalRewardAbilities[FirstAbilityValue] ? &PortalRewardAbilities[FirstAbilityValue] : nullptr;*/

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

void USATORI_GameplayAbilityComponent::CheckCooldownTimeRemaines(float TimeRemained)
{
	FTimerHandle WaitHandle;
	float localTimer = TimeRemained;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (localTimer > 0)
			{
				localTimer--;
				if (localTimer == 0)
				{
					FSATORI_AbilitiesIconsCooldownDatas CooldownData;

					if (CurrentAbilityValue == 0 && CooldownData.IsFirstCooldownAvaiable)
					{
						CooldownData.IsFirstCooldownAvaiable = false;
						CooldownAbilityIconChange.Broadcast(CooldownData);
					}

					if (CurrentAbilityValue == 1 && CooldownData.IsSecondCooldownAvaiable)
					{
						CooldownData.IsSecondCooldownAvaiable = false;
						CooldownAbilityIconChange.Broadcast(CooldownData);
					}

					if (CurrentAbilityValue == 2 && CooldownData.IsThirstCooldownAvaiable)
					{
						CooldownData.IsThirstCooldownAvaiable = false;
						CooldownAbilityIconChange.Broadcast(CooldownData);
					}
				}
			}
		}), TimeRemained, true);
}
