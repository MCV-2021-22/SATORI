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

USTRUCT(BlueprintType)
struct FSATORI_AbilitiesIconsDatas
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* CurrentAbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* NextAbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* PrevAbilitiyIcon;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAbilityIcon, const FSATORI_AbilitiesDatas&, AbilityData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAllAbilityIcon, FSATORI_AbilitiesIconsDatas, AbilityData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_GameplayAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_GameplayAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChooseAbilities")
	USATORI_ChooseAbilitiesDatas* ChoosesAbilities;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	UDataTable* AbilitiesIconDatas;

	// ----------------- // ----------------------------------------
	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbility;

	TArray<FName> PlayerAbilitiesNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAbilityValue = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int nextAbilityValue = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int PrevAbilityValue;
	// Getters
	TSubclassOf<USATORI_GameplayAbility> GetCurrentAbility();

	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAbilityIcon AbilityIconChange;

	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAllAbilityIcon AllAbilityIconChange;
	
	void NotifyAbilityChanged();
	
public:
	UFUNCTION(BlueprintCallable)
	bool TryChangeAbility();

	void SetNextAbility();
	void SetPrevAbility();

	TSubclassOf<USATORI_GameplayAbility> GetCurrentSatoriAbility();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FName AbilityName;
	TSubclassOf<USATORI_GameplayAbility> CurrentGameplayAbility;

	void AddEnabledAbilityClass(TSubclassOf<USATORI_GameplayAbility> ClassToAdd);

	bool IsAbilityClassEnabled(TSubclassOf<USATORI_GameplayAbility> ClassToCheck) const;
};
