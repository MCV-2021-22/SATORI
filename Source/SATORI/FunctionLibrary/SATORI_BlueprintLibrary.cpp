// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "SATORICharacter.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "Character/SATORI_CharacterBase.h"

float USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(AActor* DamagedActor, float BaseDamage,
	AActor* DamageCauser, TSubclassOf<UGameplayEffect> DamageEffect)
{
	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(DamagedActor);
	if (!Character)
	{ 
		UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Cant Cast DamagedActor to BaseCharacter ... "));
	} 
	else
	{
		UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
		if (!AbilitySystemComponent)
		{ 
			UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Cant get Ability System Component ... "));
		}
		else
		{
			FName DamageTag = "Data.Damage";
			FGameplayEffectContextHandle ContextHandle;
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.0f, ContextHandle);
			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			if (Character->GetEasyMode())
			{
				Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(DamageTag), 0.0f);
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
				Spec->GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(DamageTag), false, -1.0f);
			}
			else
			{
				Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(DamageTag), BaseDamage);
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
				Spec->GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(DamageTag), false, -1.0f);
			}
		}
	}
	return BaseDamage;
}

void USATORI_BlueprintLibrary::ApplyGameplayEffect(AActor* Actor, TSubclassOf<class UGameplayEffect> GameplayEffect)
{
	if (!GameplayEffect)
	{
		UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Failed to load GameplayEffect ... "));
		return;
	}

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(Actor);
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Failed Cast to ASATORI_CharacterBase ... "));
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
	if(!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Failed Cast to UAbilitySystemComponent ... "));
		return;
	}

	FGameplayEffectContextHandle ContextHandle;
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.0f, ContextHandle);
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
}

void USATORI_BlueprintLibrary::RemoveGameplayEffect(ASATORI_CharacterBase* Character, FGameplayTagContainer Tags)
{
	if (Tags.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Tag container is empty ... "));
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("USATORI_BlueprintLibrary: Failed Cast to UAbilitySystemComponent ... "));
		return;
	}

	AbilitySystemComponent->RemoveActiveEffectsWithTags(Tags);	
}