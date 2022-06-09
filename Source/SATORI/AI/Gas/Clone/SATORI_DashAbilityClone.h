//

#pragma once

#include "CoreMinimal.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORI/Character/SATORI_CharacterBase.h"
#include "SATORI_DashAbilityClone.generated.h"

/**
 * 
 */

class UCharacterMovementComponent;

UCLASS()
class SATORI_API USATORI_DashAbilityClone : public USATORI_GameplayAbility, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USATORI_DashAbilityClone();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|Dash")
	float DashDistance = 25.0f;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	FVector EnemyPosition;

private:

	const bool bStopWhenAbilityEnds = true;

	FVector Direction = FVector::FVector(1.0f, 0.0f, 0.0f); //It just works

	ASATORI_CharacterBase* Clone;

	FTransform SpawnTransform;

	bool bDashing = false;

public:

	bool bIsCreateOnRunning = false;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;
	virtual TStatId GetStatId() const override;

};
