// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SATORI_AbilityTask_StartAbilityAndWait.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityEndedDelegate);

/**
 * Activate and ability and wait for it to end
 */
UCLASS()
class SATORI_API UAbilityTask_StartAbilityAndWait : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityEndedDelegate	OnAbilityEnded;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityEndedDelegate	OnAbilityCannotBeActivated;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_StartAbilityAndWait* StartAbilityAndWaitForEnd(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<UGameplayAbility> AbilityClassToActivate);

	virtual void Activate() override;

	UPROPERTY()
	TSubclassOf<UGameplayAbility> AbilityClassToActivate;

	UFUNCTION()
	void OnAbilityEndedCallback(UGameplayAbility* EndedAbility);

	bool GetAbilityHasBeenActivated() { return bHasBeenActivated; }
	void SetAbilityHasBeenActivated(bool Value) { bHasBeenActivated = Value; }
protected:
	bool bHasBeenActivated = false;
};
