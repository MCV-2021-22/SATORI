//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "GameplayEffect.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/AbilitiesActors/SATORI_PushActor.h"
#include "SATORI_PushAbility.generated.h"

UCLASS()
class SATORI_API USATORI_PushAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_PushAbility();

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

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Push")
	float Damage = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.1"), Category = "Ability|Push")
	float TimeToEndAbility = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "100.0", UIMax = "10000.0"), Category = "Ability|Push|Debug")
	float Range = 3000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "5.0", UIMax = "85.0"), Category = "Ability|Push|Debug")
	float AngleRange = 20.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "16.0", UIMax = "128.0"), Category = "Ability|Push|Debug")
	float BoxRadius = 32.0f;
	
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
