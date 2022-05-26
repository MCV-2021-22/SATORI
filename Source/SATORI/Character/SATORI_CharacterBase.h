// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "SATORI_CharacterBase.generated.h"

class USATORI_AbilitySystemComponent;
class USATORI_AttributeSet;


UCLASS()
class SATORI_API ASATORI_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;

public:
	
	ASATORI_CharacterBase();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Character Tags Utilities

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
		float GetMaxHealth() const;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetHealth(float Health);

	TWeakObjectPtr<USATORI_AttributeSet> AttributeSetBase;

	// The core ActorComponent for interfacing with the GameplayAbilities System
	TWeakObjectPtr<USATORI_AbilitySystemComponent> AbilitySystemComponent;
};
