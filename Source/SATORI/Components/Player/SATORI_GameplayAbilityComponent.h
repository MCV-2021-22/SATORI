// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_GameplayAbilityComponent.generated.h"

class USATORI_AbilityDataAsset;
class UDataTable;

USTRUCT()
struct FSATORI_AbilitiesDatas
{
	GENERATED_BODY()

	TSubclassOf<USATORI_GameplayAbility> CurrentAbilities;
	bool isActive = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_GameplayAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_GameplayAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	UDataTable* AbilitiesIconDatas;

	UFUNCTION(BlueprintCallable)
	bool TryChangeAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<USATORI_GameplayAbility>> EnabledAbilityClasses;

	TMap<FName, FSATORI_AbilitiesDatas> PlayerGameplayAbility;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FName AbilityName;
	USATORI_GameplayAbility* CurrentGameplayAbility;

	void AddEnabledAbilityClass(TSubclassOf<USATORI_GameplayAbility> ClassToAdd);

	bool IsAbilityClassEnabled(TSubclassOf<USATORI_GameplayAbility> ClassToCheck) const;
};
