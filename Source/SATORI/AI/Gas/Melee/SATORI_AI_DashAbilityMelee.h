//

#pragma once

#include "CoreMinimal.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI/AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_AI_DashAbilityMelee.generated.h"

class UCharacterMovementComponent;

UCLASS()
class SATORI_API USATORI_AI_DashAbilityMelee : public USATORI_GameplayAbility, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USATORI_AI_DashAbilityMelee();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TSubclassOf<ASATORI_DashMeleeActor> DashMeleeActor;

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
	FGameplayTag TagStartDash;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Tags")
	FGameplayTag TagEndAbility;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.001"), Category = "Ability|Dash")
	float DashSpeed = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = "0.0"), Category = "Ability|Dash")
	float Damage = 1.0f;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

private:

	ASATORI_AICharacter* Melee;
	ASATORI_DashMeleeActor* DashActor;

	void SpawnActor();
	void GetTarget();

	bool bDashing = false;
	bool bTargeting = false;
	
	ASATORI_CharacterBase* Enemy;
	FVector  DashActorPosition;

	const bool bStopWhenAbilityEnds = true;

public:

	bool bIsCreateOnRunning = false;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;
	virtual bool IsAllowedToTick() const override;
	virtual TStatId GetStatId() const override;

};
