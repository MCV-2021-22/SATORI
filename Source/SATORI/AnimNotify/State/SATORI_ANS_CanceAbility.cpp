// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_CanceAbility.h"
#include "SATORICharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

USATORI_ANS_CanceAbility::USATORI_ANS_CanceAbility()
{
	
}

void USATORI_ANS_CanceAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(MeshComp->GetOwner());
	FGameplayTagContainer CancelAbilityTag;
	if (PlayerCharacter)
	{
		USATORI_ComboSystemComponent* CombatComponent = PlayerCharacter->GetComboSystemComponent();
		if (CombatComponent)
		{
			if (CombatComponent->isAbilityCanceled)
			{
				USATORI_AbilitySystemComponent* AbilitySystemComponent = 
					Cast<USATORI_AbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
				if (AbilitySystemComponent)
				{
					CancelAbilityTag.AddTag(TagToCancelAbility);

					AbilitySystemComponent->CancelAbilities(&CancelAbilityTag);

					StopPlayMontage(MeshComp, PlayerCharacter);
				}
			}
		}
	}
}

void USATORI_ANS_CanceAbility::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(MeshComp->GetOwner());
	FGameplayTagContainer CancelAbilityTag;
	if (PlayerCharacter)
	{
		USATORI_ComboSystemComponent* CombatComponent = PlayerCharacter->GetComboSystemComponent();
		if (CombatComponent)
		{
			if (CombatComponent->isAbilityCanceled)
			{
				USATORI_AbilitySystemComponent* AbilitySystemComponent = 
					Cast<USATORI_AbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
				if (AbilitySystemComponent)
				{
					CancelAbilityTag.AddTag(TagToCancelAbility);

					PlayerCharacter->PlayerCancelAbilityWithTag(CancelAbilityTag);

					StopPlayMontage(MeshComp, PlayerCharacter);
				}
			}
		}
	}
}

void USATORI_ANS_CanceAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

void USATORI_ANS_CanceAbility::StopPlayMontage(USkeletalMeshComponent* MeshComponent, ASATORICharacter* PlayerCharacter)
{
	/*UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
	if (AnimInstance && MontageToPlay)
	{
		FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
		if (MontageInstance)
		{
			MontageInstance->OnMontageBlendingOutStarted.Unbind();
			MontageInstance->OnMontageEnded.Unbind();
		}
	}
	if (PlayerCharacter)
	{
		USATORI_AbilitySystemComponent* AbilitySystemComponent = 
			Cast<USATORI_AbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->CurrentMontageStop();
			FGameplayTag Combo_1_Tag = FGameplayTag::RequestGameplayTag("Ability.Input.Combo1");
			FGameplayTag Combo_2_Tag = FGameplayTag::RequestGameplayTag("Ability.Input.Combo2");
			if (AbilitySystemComponent->HasMatchingGameplayTag(Combo_1_Tag))
			{
				AbilitySystemComponent->RemoveLooseGameplayTag(Combo_1_Tag);
			}
			if (AbilitySystemComponent->HasMatchingGameplayTag(Combo_2_Tag))
			{
				AbilitySystemComponent->RemoveLooseGameplayTag(Combo_2_Tag);
			}
		}
	}*/
}