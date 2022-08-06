//

#include "AI/Gas/GeneralAbilities/SATORI_AIDeathAbility.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "SATORIGameMode.h"

USATORI_AIDeathAbility::USATORI_AIDeathAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_AIDeathAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Actor = GetAvatarActorFromActorInfo();
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);

	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);

	Character->GetCharacterMovement()->Velocity = FVector(0);
	Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	Character->RemoveGameplayTag(BurstTag);
	Character->RemoveGameplayTag(AbilityTag);
	UAnimMontage* AnimMontage = Character->GetCurrentMontage();
	if (IsValid(AnimMontage))
	{
		Character->StopAnimMontage(AnimMontage);
	}

}