// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags.h"
#include "SATORI_AICharacter.generated.h"

class UGameplayEffect;
class USATORI_AttributeSet;
class USATORI_AbilitySystemComponent;
class USATORI_GameplayAbility;
class USATORI_AbilityDataAsset;
class UBehaviorTree;
class UPawnSensingComponent;


UCLASS()
class SATORI_API ASATORI_AICharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	


public:
	// Sets default values for this character's properties
	ASATORI_AICharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialize AI Attributes from GE
	void InitializeAttributes();

	// Adding Abilities to AI
	virtual void AddAICharacterAbilities();

	void GrantAbilityToPlayer(FGameplayAbilitySpec Ability);

	virtual void PossessedBy(AController* NewController) override;


public:
	UPROPERTY()
	USATORI_AttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USATORI_AbilitySystemComponent* AbilitySystemComponent;

	// Character Default Abilities Asset (Contain List of Player Abilities)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GAS")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness)
		UPawnSensingComponent* PawnSensor;

protected:
	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI|GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI|GAS")
	TArray<TSubclassOf<USATORI_GameplayAbility>> AICharacterAbilities;

	
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr <UBehaviorTree> bte;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBehaviorTree* btree;

};
