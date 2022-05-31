//

#include "Character/SATORI_CharacterBase.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"

// Sets default values

// Sets default values
ASATORI_CharacterBase::ASATORI_CharacterBase()
{

}

void ASATORI_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

//AbilitySystemComponent

UAbilitySystemComponent* ASATORI_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASATORI_CharacterBase::AddGameplayTagToAbilitySystem(const FGameplayTag& TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ASATORI_CharacterBase::RemoveGameplayTagFromAbilitySystem(const FGameplayTag& TagToRemove)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void ASATORI_CharacterBase::BlockGameplayTag(const FGameplayTagContainer& TagsToBlock)
{
	AbilitySystemComponent->BlockAbilitiesWithTags(TagsToBlock);
}

void ASATORI_CharacterBase::UnBlockGameplayTag(const FGameplayTagContainer& TagsToBlock)
{
	AbilitySystemComponent->UnBlockAbilitiesWithTags(TagsToBlock);
}



//GameplayTags
void ASATORI_CharacterBase::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	GameplayTags.AddTag(TagToAdd);
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
}

void ASATORI_CharacterBase::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
	GameplayTags.RemoveTag(TagToRemove);
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

// Getters
float ASATORI_CharacterBase::GetHealth() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetHealth();

	return 0.0f;
}

float ASATORI_CharacterBase::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetMaxHealth();

	return 0.0f;
}

float ASATORI_CharacterBase::GetDefense() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetDefense();

	return 0.0f;
}

float ASATORI_CharacterBase::GetAttack() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetAttack();

	return 0.0f;
}

float ASATORI_CharacterBase::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetMoveSpeed();

	return 0.0f;
}

float ASATORI_CharacterBase::GetGold() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetGold();

	return 0.0f;
}

int32 ASATORI_CharacterBase::GetCharacterLevel() const
{
	return 1;
}

// Setters
void ASATORI_CharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetHealth(Health);
}
