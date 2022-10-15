// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "AI/Components/Fujin/SATORI_FujinKickAereo.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Fujin/SATORI_FujinLevantarCollision.h"
#include "SATORI_DmgArea.generated.h"


/**
 * 
 */
UCLASS()
class SATORI_API USATORI_DmgArea : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_DmgArea();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagEndAbility;

	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_FujinKickAereo> ProjectileClass;

	void CheckFujin(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_FujinLevantarCollision> ProjectileClass1;

	void checkCollisionPlayer(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

protected:

	int iteracion = 0;
	int max_iteracion = 4;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	ASATORICharacter* Player2;
private:

	bool final = false;
	const bool bStopWhenAbilityEnds = true;
};
