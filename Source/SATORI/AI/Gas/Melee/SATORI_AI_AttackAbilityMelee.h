//

#pragma once

#include "CoreMinimal.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_AI_AttackAbilityMelee.generated.h"

class UCharacterMovementComponent;

UCLASS()
class SATORI_API USATORI_AI_AttackAbilityMelee : public USATORI_GameplayAbility, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USATORI_AI_AttackAbilityMelee();

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
	FGameplayTag CanMoveTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TargetingTag;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|AttackCombo")
	float MovementSpeed = 15.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|AttackCombo")
	float ClampDistanceAttack = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "1.0"), Category = "Ability|AttackCombo")
	float MaxRotationChange = 10.0f;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	ASATORI_AICharacter* Melee;

private:

	const bool bStopWhenAbilityEnds = true;

	void GetTarget();
	void RotateToTargetGradual(float DeltaTime);
	void RotateToTargetSnap();
	void MovementToTarget(float DeltaTime);

	bool RotationChanged;

	ASATORI_CharacterBase* Enemy;

public:

	bool bIsCreateOnRunning = false;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;
	virtual TStatId GetStatId() const override;

};
