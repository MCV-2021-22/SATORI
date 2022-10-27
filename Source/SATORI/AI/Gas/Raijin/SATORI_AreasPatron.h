// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageandWaitNotify.h"
#include "AI/Components/Raijin/SATORI_RaijinRayoMovil.h"
#include "SATORI_AreasPatron.generated.h"


class UParticleSystem;

UCLASS()
class SATORI_API USATORI_AreasPatron : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_AreasPatron();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagEndAbility;
	
	// Particle
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* Area_Particle = nullptr;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<ASATORI_RaijinRayoMovil> ProjectileClass;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	void EndRayos(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Missile")
		float Damage = 0.001;


protected:
	TArray< AActor* > Rayos;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
private:

	const bool bStopWhenAbilityEnds = true;
};
