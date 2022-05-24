// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SATORI_CharacterBase.h"
#include "GAS/SATORI_AbilitySystemComponent.h"

// Sets default values
ASATORI_CharacterBase::ASATORI_CharacterBase()
{

}

// Called when the game starts or when spawned
void ASATORI_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ASATORI_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASATORI_CharacterBase::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ASATORI_CharacterBase::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void ASATORI_CharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}

bool ASATORI_CharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return GameplayTags.HasTag(TagToCheck);
}

bool ASATORI_CharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return GameplayTags.HasAll(TagContainer);
}

bool ASATORI_CharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return GameplayTags.HasAny(TagContainer);
}

void ASATORI_CharacterBase::BlockGameplayTag(const FGameplayTagContainer& TagsToBlock)
{
	AbilitySystemComponent->BlockAbilitiesWithTags(TagsToBlock);
}

void ASATORI_CharacterBase::UnBlockGameplayTag(const FGameplayTagContainer& TagsToBlock)
{
	AbilitySystemComponent->UnBlockAbilitiesWithTags(TagsToBlock);
}