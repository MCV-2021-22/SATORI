//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_AI_BlockAbilityMelee.generated.h"

class UCharacterMovementComponent;

UCLASS()
class SATORI_API USATORI_AI_BlockAbilityMelee : public USATORI_GameplayAbility, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USATORI_AI_BlockAbilityMelee();

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
	FGameplayTag BlockDamageTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag CanBeStunnedTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag BlockingTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag HitTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag  LaunchTag;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1"), Category = "Ability|Block")
	float TimeToFinish = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1"), Category = "Ability|Block")
	int RotationDifference = 2;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

private:

	ASATORI_AICharacter* Melee;
	AActor* Enemy;

	void GetTarget();

	void RemoveGameplayEffect();
	void EndBlockingAnimation();
	void BlockDamage();

	float RotationRate;

	bool bBlocking = false;

	const bool bStopWhenAbilityEnds = true;


public:

	bool bIsCreateOnRunning = false;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;
	virtual TStatId GetStatId() const override;

};
