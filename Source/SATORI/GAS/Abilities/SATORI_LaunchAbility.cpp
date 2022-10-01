// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_LaunchAbility.h"
#include "SATORICharacter.h"
#include "AbilitySystemComponent.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "GAS/Tasks/SATORI_AbilityTask_StartAbilityAndWait.h"
#include "FunctionLibrary/AsyncTaskCooldownChanged.h"
#include "GameplayEffect.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"

void USATORI_LaunchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ASATORICharacter* Player = Cast<ASATORICharacter>(ActorInfo->AvatarActor);

	if (Player)
	{
		USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Player->GetPlayerAbilityComponent();
		if (CurrentAbilityComponent)
		{
			TSubclassOf<USATORI_GameplayAbility> NextAbility = CurrentAbilityComponent->GetCurrentSatoriAbility();
			if (NextAbility)
			{
				UAbilityTask_StartAbilityAndWait* CurrentStartAbility = UAbilityTask_StartAbilityAndWait::StartAbilityAndWaitForEnd(this, FName(""), NextAbility);
				if (CurrentStartAbility)
				{
					CurrentStartAbility->Activate();
					if (CurrentStartAbility->GetAbilityHasBeenActivated())
					{
						ApplyCooldownToAbilityUI(NextAbility, Player);
					}
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void USATORI_LaunchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_LaunchAbility::ApplyCooldownToAbilityUI(TSubclassOf<USATORI_GameplayAbility> Ability, ASATORICharacter* Player)
{
	// Test
	USATORI_GameplayAbility* PlayerAbility = Cast<USATORI_GameplayAbility>(Ability->GetDefaultObject());
	const FGameplayEffectSpec* Spec = nullptr;
	UAbilitySystemComponent* AbilitySystemComponent = Player->GetAbilitySystemComponent();
	
	float TimeRemaining = 0.0f;
	float CooldownDuration = 0.0f;

	if (PlayerAbility)
	{
		UGameplayEffect* CooldownEffect = PlayerAbility->GetCooldownGameplayEffect();
		if (CooldownEffect)
		{
			TSubclassOf<UGameplayEffect> PlayerCooldownGameplayEffect = CooldownEffect->GetClass();
			FGameplayEffectContextHandle ContextHandle;
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PlayerCooldownGameplayEffect, 1.0f, ContextHandle);
			Spec = SpecHandle.Data.Get();

			// Facking this sh*t man bro omg, sh*t fack
			FGameplayTagContainer InCooldownTags = FGameplayTagContainer();
			Spec->GetAllGrantedTags(InCooldownTags);
			FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTags);
			TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
			if (DurationAndTimeRemaining.Num() > 0)
			{
				int32 BestIdx = 0;
				float LongestTime = DurationAndTimeRemaining[0].Key;
				for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
				{
					if (DurationAndTimeRemaining[Idx].Key > LongestTime)
					{
						LongestTime = DurationAndTimeRemaining[Idx].Key;
						BestIdx = Idx;
					}
				}

				TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
				CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;
			}
		}
	}

	const FGameplayTagContainer* CooldownTag = Spec->CapturedSourceTags.GetAggregatedTags();
	/*UAsyncTaskCooldownChanged* CooldownChanged =
		UAsyncTaskCooldownChanged::ListenForCooldownChange(Player->GetAbilitySystemComponent(), *CooldownTag, true);*/

	//UE_LOG(LogTemp, Warning, TEXT("Time Remain, %f"), TimeRemaining);
	USATORI_GameplayAbilityComponent* CurrentAbilityComponent = Player->GetPlayerAbilityComponent();
	if (CurrentAbilityComponent)
	{
		CurrentAbilityComponent->NotifyCooldownAbilityChanged(TimeRemaining);
	}
}