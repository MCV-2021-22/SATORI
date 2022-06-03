// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerState.h"

USATORI_GameplayAbility::USATORI_GameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

ASATORICharacter* USATORI_GameplayAbility::GetOwningSatoriCharacter() const
{
	ASATORI_PlayerState* PS = Cast<ASATORI_PlayerState>(GetOwningActorFromActorInfo());
	if (PS)
	{
		ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(PS->GetPawn());
		if (PlayerCharacter)
			return PlayerCharacter;
	}
	return nullptr;
}

FName USATORI_GameplayAbility::GetAbilityName() const
{
	return AbilityName;
}