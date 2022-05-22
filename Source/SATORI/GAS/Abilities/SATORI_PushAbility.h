//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/Push/SATORI_PushActor.h"
#include "SATORI_PushAbility.generated.h"

/**
 * 
 */
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
		override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FName TagSpawnAbility = FName(TEXT("Event.Montage.SpawnAbility"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FName TagEndAbility = FName(TEXT("Event.Montage.EndAbility"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FName PlayerTargetingTag = FName(TEXT("State.Targeting"));

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|Push")
	float Speed = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|Push")
	float PushForce = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|Push")
	float TimeToDestroy = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "100.0", UIMax = "10000.0"), Category = "Ability|Push")
	float Range = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "5.0", UIMax = "85.0"), Category = "Ability|Push")
	float AngleRange = 5.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (UIMin = "16.0", UIMax = "128.0"), Category = "Ability|Push")
	float SphereRadius = 32.0f;

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
