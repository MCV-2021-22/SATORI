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
class USATORI_GameInstance;

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
	bool isUpgrated = false;
};

USTRUCT(BlueprintType)
struct FSATORI_AbilitiesBordesChecker
{
	GENERATED_BODY()

	bool IsFirstIconAvaiable = false;
	bool IsSecondIconAvaiable = false;
	bool IsThirstIconAvaiable = false;
};

USTRUCT(BlueprintType)
struct FSATORI_AbilitiesIconsDatas
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* FirstAbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* SecondAbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* LastAbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSATORI_AbilitiesBordesChecker AbilitiesBordesChecker;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAbilityIcon, const FSATORI_AbilitiesDatas&, AbilityData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAllAbilityIcon, FSATORI_AbilitiesIconsDatas, AbilityData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_GameplayAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USATORI_GameplayAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Abilities")
	TArray<TSubclassOf<class USATORI_GameplayAbility>> SATORIAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChooseAbilities")
	USATORI_ChooseAbilitiesDatas* ChoosesAbilities;

	// ----------------- // ----------------------------------------
	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbility;

	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbilityDissabled;

	TArray<FName> PlayerAbilitiesNames;

	TArray<FName> PlayerAbilitiesNamesDissabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* EmptyAbilitiyIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAbilityValue = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsNachoTesting = false;

	// Getters

	TArray<FSATORI_AbilitiesDatas> GetCharacterAbilities() { return PortalRewardAbilities; }

	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAbilityIcon AbilityIconChange;

	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAllAbilityIcon AllAbilityIconChange;
	
	void NotifyAbilityChanged();
	
	void ResetCurrentPlayerAbilities();
public:
	UFUNCTION(BlueprintCallable)
	bool TryChangeAbility();

	void SetNextAbility();
	void SetPrevAbility();

	TSubclassOf<USATORI_GameplayAbility> GetCurrentSatoriAbility();

	void SetSavedAbilitiesWithGameInstance(USATORI_GameInstance* GameInstance);
protected:
	
	virtual void BeginPlay() override;
private:

	FName AbilityName;
	TSubclassOf<USATORI_GameplayAbility> CurrentGameplayAbility;

	// Abilities
	TArray<FSATORI_AbilitiesDatas> PortalRewardAbilities;

	ASATORICharacter* PlayerCharacter = nullptr;

	// Abilites Datas
	const FSATORI_AbilitiesDatas* SecondAbilityData = nullptr;
	const FSATORI_AbilitiesDatas* LastAbilityData = nullptr;
	const FSATORI_AbilitiesDatas* FirstAbilityData = nullptr;

	UPROPERTY()
	FSATORI_AbilitiesIconsDatas AbilityIconToChange;
public:

	void AddPortalAbilities(FSATORI_AbilitiesDatas AbilityData);

	void RemoveEnabledAbility();

	bool IsAbilityEnabled() const;

	void CheckAbilitiesStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas);
	void CheckAbilitiesBorderStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas);
};
