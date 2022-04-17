// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SATORI_StatsComponent.generated.h"

class ASATORICharacter;
class ASATORI_PlayerState;
class USATORI_AttributeSet;
class UAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_StatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_StatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeHealthAttribute(ASATORI_PlayerState* PlayerState);
	virtual void BindAttributeChage(ASATORICharacter* PlayerCharacter);

	// Health
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
protected:

	TWeakObjectPtr<USATORI_AttributeSet> PlayerAttributes;
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attributes
	float MaxHealth = 0.f;
	float Health = 0.f;

	// Delegates
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
};
