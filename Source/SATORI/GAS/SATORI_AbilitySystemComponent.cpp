// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AbilitySystemComponent.h"
#include "Attributes/SATORI_AttributeSet.h"

//void USATORI_AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
//{
//	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
//
//	if (AbilityActorInfo)
//	{
//		if (UGameInstance* GameInstance = InOwnerActor->GetGameInstance())
//		{
//			// Sign up for possess/unpossess events so that we can update the cached AbilityActorInfo accordingly
//			GameInstance->GetOnPawnControllerChanged().AddDynamic(this, &USATORI_AbilitySystemComponent::OnPawnControllerChanged);
//		}
//	}
//}

bool USATORI_AbilitySystemComponent::SatoriCheckIsAbilityActive(const FGameplayTagContainer* WithTags, UGameplayAbility* Ignore)
{
	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (!Spec.IsActive() || Spec.Ability == nullptr || Spec.Ability == Ignore)
		{
			continue;
		}

		const bool WithTagPass = (!WithTags || Spec.Ability->AbilityTags.HasAny(*WithTags));

		if (WithTagPass)
		{
			return true;
		}
	}
	return false;
}

void USATORI_AbilitySystemComponent::OnPawnControllerChanged(APawn* Pawn, AController* NewController)
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor == Pawn && AbilityActorInfo->PlayerController != NewController)
	{
		// Reinit the cached ability actor info (specifically the player controller)
		AbilityActorInfo->InitFromActor(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get(), this);
	}
}

void USATORI_AbilitySystemComponent::BeginDestroy()
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor.IsValid())
	{
		if (UGameInstance* GameInstance = AbilityActorInfo->OwnerActor->GetGameInstance())
		{
			GameInstance->GetOnPawnControllerChanged().RemoveAll(this);
		}
	}

	Super::BeginDestroy();
}