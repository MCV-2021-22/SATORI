// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_GameplayAbilityComponent.generated.h"

class USATORI_ChooseAbilitiesDatas;
class USATORI_AbilityDataAsset;
class USATORI_ChangeAbilitiesWidget;
class UDataTable;

USTRUCT(BlueprintType)
struct FSATORI_AbilitiesDatas
{
	GENERATED_BODY()

	TSubclassOf<USATORI_GameplayAbility> CurrentAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* AbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool isActive = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAbilityIcon, const FSATORI_AbilitiesDatas&, AbilityData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_GameplayAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USATORI_GameplayAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	UDataTable* AbilitiesIconDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChooseAbilities")
	USATORI_ChooseAbilitiesDatas* ChoosesAbilities;

	/*UFUNCTION(BlueprintCallable)
	bool TryChangeAbilityIcon();*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<USATORI_GameplayAbility>> EnabledAbilityClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<USATORI_GameplayAbility>> DisabledAbilityClasses;

	// ----------------- // ----------------------------------------
	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbility;

	TArray<FName> PlayerAbilitiesNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAbilityValue = 0;

	// Getters
	TSubclassOf<USATORI_GameplayAbility> GetCurrentAbility();

	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAbilityIcon AbilityIconChange;
	
	void NotifyAbilityChanged();
	
public:
	UFUNCTION(BlueprintCallable)
	bool TryChangeAbility();

	//FSATORI_AbilitiesDatas* SetNextAbility();

	//void UpdateAbilityIcon();
	void SetNextAbility();
	TSubclassOf<USATORI_GameplayAbility> GetCurrentSatoriAbility();
protected:
	
	virtual void BeginPlay() override;

private:

	FName AbilityName;
	TSubclassOf<USATORI_GameplayAbility> CurrentGameplayAbility;

public:

	void AddEnabledAbilityClass(TSubclassOf<USATORI_GameplayAbility> ClassToAdd);

	void RemoveEnabledAbilityClass(TSubclassOf<USATORI_GameplayAbility> ClassToRemove);

	bool IsAbilityClassEnabled(TSubclassOf<USATORI_GameplayAbility> ClassToCheck) const;
};
