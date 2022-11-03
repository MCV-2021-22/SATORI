//

#include "Character/SATORI_CharacterBase.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "GAS/SATORI_AbilitySystemComponent.h"

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

void ASATORI_CharacterBase::AddGameplayTag(const FGameplayTag& TagToAdd)
{
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	}
}

void ASATORI_CharacterBase::RemoveGameplayTag(const FGameplayTag& TagToRemove)
{
	if (AbilitySystemComponent.IsValid())
		AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void ASATORI_CharacterBase::BlockGameplayTag(const FGameplayTagContainer& TagsToBlock)
{
	if (AbilitySystemComponent.IsValid())
		AbilitySystemComponent->BlockAbilitiesWithTags(TagsToBlock);
}

void ASATORI_CharacterBase::UnBlockGameplayTag(const FGameplayTagContainer& TagsToBlock)
{
	if (AbilitySystemComponent.IsValid())
		AbilitySystemComponent->UnBlockAbilitiesWithTags(TagsToBlock);
}

void ASATORI_CharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent.IsValid())
		AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool ASATORI_CharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}

bool ASATORI_CharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool ASATORI_CharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;

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

float ASATORI_CharacterBase::GetMana() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetMana();

	return 0.0f;
}

float ASATORI_CharacterBase::GetMaxMana() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetMaxMana();

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

bool ASATORI_CharacterBase::IsDead() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetHealth() > 0;

	return false;
}

// Setters
void ASATORI_CharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetHealth(Health);
}

void ASATORI_CharacterBase::SetMana(float Mana)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetMana(Mana);
}

void ASATORI_CharacterBase::SetMaxHealth(float MaxHealth)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetMaxHealth(MaxHealth);
}

void ASATORI_CharacterBase::SetMaxMana(float MaxMana)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetMaxMana(MaxMana);
}

void ASATORI_CharacterBase::SetDefense(float Defense)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetDefense(Defense);
}

void ASATORI_CharacterBase::SetAttack(float Attack)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetAttack(Attack);
}

void ASATORI_CharacterBase::SetMoveSpeed(float MoveSpeed)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetMoveSpeed(MoveSpeed);
}

void ASATORI_CharacterBase::SetGold(float Gold)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetGold(Gold);
}
