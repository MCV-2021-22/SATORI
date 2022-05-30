//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SATORI_DashAbility.generated.h"

/**
 * 
 */

class UCharacterMovementComponent;

//FTickableObject
//Tick
//IsTickable
//ISallowedtotick
//getstatID
//TSTATID getstatID Delegate

UCLASS()
class SATORI_API USATORI_DashAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_DashAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, 
		bool bWasCancelled)
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagEndAbility;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|Dash")
	float DashDistance = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.001", UIMax = "1.0"), Category = "Ability|Dash")
	float DashSpeed = 0.01f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1"), Category = "Ability|Dash")
	int CallTracker = 50;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

private:

	const bool bStopWhenAbilityEnds = true;

	FVector Direction = FVector::FVector(1.0f, 0.0f, 0.0f); //It just works

	FTimerHandle TimerHandleDash;

	int CallTrackerRegistry;

	void Dashing();
};
