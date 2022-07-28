//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "GameplayEffect.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/AbilitiesActors/SATORI_PushActor.h"
#include "SATORI_Push360Ability.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_Push360Ability : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_Push360Ability();
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
		FGameplayTag TagEndAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Push")
	float Damage = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Push")
	float Speed = 6000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Push")
	float PushForce = 6000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Push")
	float TimeToDestroy = 0.2f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "100.0", UIMax = "10000.0"), Category = "Ability|Push")
	float Range = 3000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "16.0", UIMax = "128.0"), Category = "Ability|Push")
	float SphereRadius = 32.0f;

protected:

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	
private:

	const bool bStopWhenAbilityEnds = true;

};
