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
class UUserWidget;
class ASATORI_PlayerController;
class USATORI_GameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIPlayerIsDead, bool, isDead);

/*
* Class represent All Player Stats, we can use for UI 
* To listen for when an Attribute changes to update the UI or other gameplay, 
* use UAbilitySystemComponent::GetGameplayAttributeValueChangeDelegate(FGameplayAttribute Attribute)
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_StatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_StatsComponent();

	// Initialize all Health attributes for local values
	virtual void InitializeStatsAttributes(ASATORI_PlayerState* PlayerState);
	virtual void InitializeStatsAttributesByInstance(ASATORI_PlayerState* PlayerState, USATORI_GameInstance* GameInstance);

	// For crash when enemy disolver and player death at same time when return to lobby
	FSATORIPlayerIsDead FOnPlayerDeathBroadCast;
	void OnPlayerDeathBroadCastForDissolver();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerStat();

	// Helper function if we change some value attributes and we capture the changes
	virtual void BindAttributeChage(ASATORICharacter* PlayerCharacter);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Health Functions
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	//Mana Functions
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

	//Pandacoins
	virtual void PandaCoinChanged(const FOnAttributeChangeData& Data);
	
protected:

	// Attributes 
	TWeakObjectPtr<USATORI_AttributeSet> PlayerAttributes;
	// Ability Component
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attributes
	float MaxHealth = 0.f;
	float Health = 0.f;

	float MaxMana = 0.f;
	float Mana = 0.f;

	float PandaCoins = 0.0f;

	float MoveSpeed = 0.0f;
	float Defense = 0.0f;
	float Attack = 0.0f;

	// Delegates
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;

	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;

	FDelegateHandle PandaCoinChangedDelegateHandle;
	
	// Update UI
	void UpdateHealthBarPercent();
	void UpdateHealthBarText();

	void UpdateManaBarPercent();
	void UpdateManaBarText();

	void UpdatePandaCoinText();
};
