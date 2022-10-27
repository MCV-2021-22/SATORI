// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "SATORI_CharacterBase.generated.h"

class USATORI_AbilitySystemComponent;
class USATORI_AttributeSet;
class UCapsuleComponent;
class UAnimMontage;
class USATORI_GameplayAbility;

USTRUCT(BlueprintType) struct FDialogueStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BossNum;
};

UCLASS()
class SATORI_API ASATORI_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	ASATORI_CharacterBase();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Character Tags Utilities

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Tags")
	void AddGameplayTag(const FGameplayTag& TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Tags")
	void RemoveGameplayTag(const FGameplayTag& TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "Tags")
	void BlockGameplayTag(const FGameplayTagContainer& TagsToBlock);

	UFUNCTION(BlueprintCallable, Category = "Tags")
	void UnBlockGameplayTag(const FGameplayTagContainer& TagsToBlock);

	UFUNCTION(BlueprintCallable, Category = "Tags")
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	UFUNCTION(BlueprintCallable, Category = "Tags")
	bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const;

	UFUNCTION(BlueprintCallable, Category = "Tags")
	bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const;

	UFUNCTION(BlueprintCallable, Category = "Tags")
	bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCapsuleComponent* AttackingCollision;

	UFUNCTION()
	virtual void CharacterDeath() {};

	UFUNCTION()
	virtual void RemoveCharacterAbilities() {};

	//Tag 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tags")
	FGameplayTag DeadTag;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetHealth(float Health);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetMana(float Mana);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetMaxMana(float MaxMana);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetDefense(float Defense);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetAttack(float Attack);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetMoveSpeed(float MoveSpeed);

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	virtual void SetGold(float Gold);

	// Cheat Function
	void SetEasyMode(bool Value) { IsGodMode = Value; }
	bool GetEasyMode() { return IsGodMode; }


protected:

	virtual void BeginPlay() override;

	bool IsGodMode = false;

	TWeakObjectPtr<USATORI_AttributeSet> AttributeSetBase;

	// The core ActorComponent for interfacing with the GameplayAbilities System
	TWeakObjectPtr<USATORI_AbilitySystemComponent> AbilitySystemComponent;

	TArray<TSubclassOf<USATORI_GameplayAbility>> RemovedgameplayAbilities;

	FGameplayTag EffectRemoveOnDeathTag;
};
