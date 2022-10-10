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
class UAsyncTaskCooldownChanged;

// ------------------ Hability Datas ----------------------

USTRUCT(BlueprintType)
struct FSATORI_AbilitiesDatas
{
	GENERATED_BODY()

	TSubclassOf<USATORI_GameplayAbility> CurrentAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* AbilitiyIcon = nullptr;

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
	UTexture2D* FirstAbilitiyIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* SecondAbilitiyIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* LastAbilitiyIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSATORI_AbilitiesBordesChecker AbilitiesBordesChecker;
};

// ------------- Cooldown Datas ------------------------------
USTRUCT(BlueprintType)
struct FSATORI_CooldownDatas
{
	GENERATED_BODY()

	bool IsCooldownAvaiable = false;
	float TimeRemained = 0.0f;
	int AbilityID = 0;
};

USTRUCT(BlueprintType)
struct FSATORI_AbilitiesIconsCooldownDatas
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSATORI_CooldownDatas FirstIconDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSATORI_CooldownDatas SecondIconDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSATORI_CooldownDatas ThirstIconDatas;
};

struct FAbilityCooldownTimerInfo
{
	FTimerHandle WaitHandle;
	FTimerDelegate CooldownDelegate;
	float TimeRemaining;
	int AbilityID;
	bool IsInCooldown = false;
};

// -------------------- End Datas Sections ----------------------

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAbilityIcon, const FSATORI_AbilitiesDatas&, AbilityData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIChangeAllAbilityIcon, FSATORI_AbilitiesIconsDatas, AbilityData);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORICooldownAbilityIcon, FSATORI_AbilitiesIconsCooldownDatas, CooldownData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORICooldownAbilityIcon, FSATORI_CooldownDatas, CooldownData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_GameplayAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USATORI_GameplayAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Abilities")
	TArray<TSubclassOf<class USATORI_GameplayAbility>> SATORIAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChooseAbilities")
	USATORI_ChooseAbilitiesDatas* ChoosesAbilities = nullptr;

	// ----------------- // ----------------------------------------
	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbility;

	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbilityDissabled;

	TArray<FName> PlayerAbilitiesNames;

	TArray<FName> PlayerAbilitiesNamesDissabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* EmptyAbilitiyIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAbilityValue = 0;

	// Getters

	TArray<FSATORI_AbilitiesDatas> GetCharacterAbilities() { return PortalRewardAbilities; }

	UFUNCTION(BlueprintCallable)
	UAsyncTaskCooldownChanged* GetAsyncTaskCooldownChanged() { return CooldownIconChanges; }
	UFUNCTION(BlueprintCallable)
	void SetAsyncTaskCooldownChanged(UAsyncTaskCooldownChanged* Value) { CooldownIconChanges = Value; }

	// Notifies
	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAbilityIcon AbilityIconChange;

	UPROPERTY(BlueprintAssignable)
	FSATORIChangeAllAbilityIcon AllAbilityIconChange;

	UPROPERTY(BlueprintAssignable)
	FSATORICooldownAbilityIcon CooldownAbilityIconChange;
	
	void NotifyAbilityChanged();
	void NotifyCooldownAbilityChanged(float TimeRemaining);

	// -------------------End Notify ----------------------------

	// Reset Player Hability
	void ResetCurrentPlayerAbilities();

	// Hability Testing
	void UnclockAllHabilities(bool Value) { IsAllHabilityTesting = Value; }
public:
	UFUNCTION(BlueprintCallable)
	bool TryChangeAbility();

	void SetNextAbility();
	void SetPrevAbility();

	TSubclassOf<USATORI_GameplayAbility> GetCurrentSatoriAbility();

	void SetSavedAbilitiesWithGameInstance(USATORI_GameInstance* GameInstance);
protected:
	
	virtual void BeginPlay() override;

	void CheckCooldownTimeRemaines(int HabilityID);
	void CooldownCountDown(int HabilityID);

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

	UPROPERTY()
	UAsyncTaskCooldownChanged* CooldownIconChanges;

	// Cooldown Datas
	UPROPERTY()
	FSATORI_AbilitiesIconsCooldownDatas CooldownData;

	// Test Code
	TArray<FSATORI_CooldownDatas> M_CooldownDatas;

	//UPROPERTY()
	//float HabilityTimeRemained = 0.0f;

	//// Handle
	//FTimerHandle Hability_1_WaitHandle;

	TArray<FAbilityCooldownTimerInfo> AbilityCD_Infos;

	// Hability Test
	bool IsAllHabilityTesting = false;
public:

	void AddPortalAbilities(FSATORI_AbilitiesDatas AbilityData);

	void RemoveEnabledAbility();

	bool IsAbilityEnabled() const;

	void CheckAbilitiesStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas);
	void CheckAbilitiesBorderStatus(FSATORI_AbilitiesIconsDatas& AbilitiesDatas);
};
