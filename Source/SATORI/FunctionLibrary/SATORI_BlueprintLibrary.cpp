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
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(DamageTag), BaseDamage);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
			Spec->GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(DamageTag), false, -1.0f);
		}
	}
	return BaseDamage;
}