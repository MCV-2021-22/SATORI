//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_AIBossDowned.generated.h"

UCLASS()
class SATORI_API USATORI_AIBossDowned : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_AIBossDowned();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		UAnimMontage* AnimMontage;

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
