//

#include "AI/Tasks/BTTaksGenerales/SATORI_AIDeath.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "SATORIGameMode.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"

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
	
	/*ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->EnemyDissolveAfterDeath(200);
	}*/
}


void USATORI_AIDeath::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		
		AActor* Actor = GetAvatarActorFromActorInfo();
		
		ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
		

		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Actor);
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(Actor);

		if (Fujin)
		{
			if(Fujin->getDead())
			{
				Character->SetActorEnableCollision(false);
				Character->SpawnCointActorAfterDeath();
				//Character->EnemyDissolveAfterDeath(7.5);
				Fujin->setDead(false);
				FTimerHandle WaitHandle;
				GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
					{
						AActor* Actor1 = GetAvatarActorFromActorInfo();
						if (Actor1)
						{
							ASATORI_AICharacter* Character1 = Cast<ASATORI_AICharacter>(Actor1);

							if (Character1) {
								GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor1);
								Character1->SpawnCointActorAfterDeath();
								Character1->canDestroy = true;
								GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
							}
						}
					}), 4.0f, false);
			}
			
		}
		else if (Raijin)
		{
			if(Raijin->getDead())
			{
				Character->SetActorEnableCollision(false);
				Character->SpawnCointActorAfterDeath();
				//Character->EnemyDissolveAfterDeath(7.5);
				Raijin->setDead(false);
				FTimerHandle WaitHandle;
				GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
					{
						AActor* Actor1 = GetAvatarActorFromActorInfo();
						if (Actor1)
						{
							ASATORI_AICharacter* Character1 = Cast<ASATORI_AICharacter>(Actor1);

							if (Character1) {
								GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor1);
								Character1->SpawnCointActorAfterDeath();
								Character1->canDestroy = true;
								GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
							}
						}
					}), 4.0f, false);
			}
			
		}
		else
		{

			Character->SetActorEnableCollision(false);
			Character->SpawnCointActorAfterDeath();
			Character->EnemyDissolveAfterDeath(7.5);
			FTimerHandle WaitHandle;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (GetAvatarActorFromActorInfo())
					{
						AActor* Actor1 = GetAvatarActorFromActorInfo();
						if (Actor1)
						{
							ASATORI_AICharacter* Character1 = Cast<ASATORI_AICharacter>(Actor1);

							if (Character1) {
								GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor1);

								Character1->canDestroy = true;
								GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
							}
						}	
					}



				}), 5.0f, false);
		}
		
		//Character->canDestroy = true;

		//Character->SpawnCointActorAfterDeath();
		//Character->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));

		//Actor->Destroy();
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void USATORI_AIDeath::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
void USATORI_AIDeath::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//AActor* Actor = GetAvatarActorFromActorInfo();
	//GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}