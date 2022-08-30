// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Fujin/SATORI_FujinLevantarCollision.h"
#include "SATORI_PunchLevantar.generated.h"


class ASATORICharacter;
/**
 * 
 */
UCLASS()
class SATORI_API USATORI_PunchLevantar : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_PunchLevantar();



	FTimerDelegate TimerDelegate;

	FTimerHandle TimerHandle;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_FujinLevantarCollision> ProjectileClass;

	

	void checkCollisionPlayer(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

protected:

	int iteracion = 0;
	int max_iteracion = 4;

	ASATORICharacter* Player;
};
