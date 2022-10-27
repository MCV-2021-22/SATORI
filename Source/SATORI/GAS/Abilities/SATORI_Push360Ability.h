//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "GameplayEffect.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/AbilitiesActors/SATORI_Push360Actor.h"
#include "SATORI/Character/SATORI_CharacterBase.h"
#include "SATORI_Push360Ability.generated.h"

UCLASS()
class SATORI_API USATORI_Push360Ability : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_Push360Ability();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_Push360Actor> Push360Actor;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Push360")
	float Damage = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.1"), Category = "Ability|Push360")
	float TimeToEndAbility = 0.1f;

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

	ASATORI_CharacterBase* Character;

};
