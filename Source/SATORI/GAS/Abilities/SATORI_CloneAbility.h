//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORI/AI/Character/Clone/Satori_CloneCharacter.h"
#include "SATORI_CloneAbility.generated.h"

UCLASS()
class SATORI_API USATORI_CloneAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()
	
public:

	USATORI_CloneAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_CloneCharacter> CloneCharacter;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagSpawnAbility;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.1"), Category = "Ability|Clone")
	float TimeToEndAbility = 10.0f;

	UFUNCTION()
	void FinishWaitingForEnd();

	UFUNCTION()
		void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

private:

	const bool bStopWhenAbilityEnds = true;

};
