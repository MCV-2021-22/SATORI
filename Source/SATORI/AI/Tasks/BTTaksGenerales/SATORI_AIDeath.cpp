//

#include "AI/Tasks/BTTaksGenerales/SATORI_AIDeath.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORIGameMode.h"

USATORI_AIDeath::USATORI_AIDeath()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_AIDeath::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AIDeath::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AIDeath::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AIDeath::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AIDeath::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AIDeath::EventReceived);
	Task->ReadyForActivation();
	
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->EnemyDissolveAfterDeath();
	}
}


void USATORI_AIDeath::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		AActor* Actor = GetAvatarActorFromActorInfo();
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);
		ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
		Character->canDestroy = true;

		Character->SpawnCointActorAfterDeath();

		//Actor->Destroy();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void USATORI_AIDeath::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
void USATORI_AIDeath::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AActor* Actor = GetAvatarActorFromActorInfo();
	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}