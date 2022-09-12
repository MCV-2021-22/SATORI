// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_EnemyKnockBackAbility.generated.h"

class ASATORI_PushActor;
class UGameplayEffect;
class UAnimMontage;

UCLASS()
class SATORI_API USATORI_EnemyKnockBackAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()
	
public:
	USATORI_EnemyKnockBackAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_PushActor> PushActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagSpawnAbility;

protected:
	UFUNCTION()
	void FinishWaitingForEnd();

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.1"), Category = "Ability|KnockBack")
	float TimeToEndAbility = 0.1f;
private:

	const bool bStopWhenAbilityEnds = true;
};
