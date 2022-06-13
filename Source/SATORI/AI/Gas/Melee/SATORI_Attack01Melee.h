//

#pragma once

#include "CoreMinimal.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORI/Character/SATORI_CharacterBase.h"
#include "SATORI_Attack01Melee.generated.h"

/**
 * 
 */

class UCharacterMovementComponent;

UCLASS()
class SATORI_API USATORI_Attack01Melee : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_Attack01Melee();

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
		bool bWasCancelled) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagSpawnAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagEndAbility;

protected:

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	ASATORI_CharacterBase* Melee;

private:

	const bool bStopWhenAbilityEnds = true;

};
