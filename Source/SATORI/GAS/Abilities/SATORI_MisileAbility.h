//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/Misile/SATORI_MisileActor.h"
#include "SATORI_MisileAbility.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_MisileAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_MisileAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_MisileActor> MisileActor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability|Misile")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability|Misile")
	float TimeToDestroy;

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

private:

	const bool bStopWhenAbilityEnds = true;
	
	FTransform SpawnTransform;

};
