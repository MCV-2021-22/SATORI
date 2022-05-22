//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "Actors/BlackHole/SATORI_BlackHoleActor.h"
#include "SATORI_BlackHoleAbility.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_BlackHoleAbility : public USATORI_GameplayAbility
{
	GENERATED_BODY()

public:

	USATORI_BlackHoleAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_BlackHoleActor> BlackHoleActor;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|BlackHole")
	float Speed = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|BlackHole")
	float SphereRadiusOnExplosion = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|BlackHole")
	float TimeToDestroy = 1.0f;

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
