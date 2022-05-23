// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SATORI_GameplayAbilityComponent.generated.h"

class USATORI_AbilityDataAsset;
class UGameplayAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_GameplayAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_GameplayAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UFUNCTION(BlueprintCallable)
	bool TryChangeAbility();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FName AbilityName;
	UGameplayAbility* CurrentGameplayAbility;

	TMap<FName, TSubclassOf<UGameplayAbility>> PlayerGameplayAbility;
};
