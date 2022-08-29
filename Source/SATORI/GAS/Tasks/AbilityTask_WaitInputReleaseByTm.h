// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitInputReleaseByTm.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputReleaseByTimeDelegate, float, TotalTimeHeld);

/**
 * 
 */
UCLASS()
class SATORI_API UAbilityTask_WaitInputReleaseByTm : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAbilityTask_WaitInputReleaseByTm(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FInputReleaseByTimeDelegate	OnRelease;

	UFUNCTION()
	void OnReleaseByTimeCallback();

	virtual void Activate() override;

	void UpdateHoldTime();

	/** Wait until the user releases the input button for this ability's activation by Timer. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitInputReleaseByTm* WaitInputReleaseByTime(UGameplayAbility* OwningAbility, bool bTestAlreadyReleased = false, float MaxHoldTime = 3.0f);

protected:

	float StartTime;
	float HoldTime;
	float MaxHoldTime;
	bool bTestInitialState;
	FDelegateHandle DelegateHandle;

	void CountMaxHoldTime();
};
