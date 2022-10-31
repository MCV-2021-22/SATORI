//

#include "AI/Tasks/BTTaksGenerales/SATORI_AIBossRevivir.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "AbilityTask/SATORI_PlayMontageAndWaitEvent.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "SATORIGameMode.h"

USATORI_AIBossRevivir::USATORI_AIBossRevivir()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_AIBossRevivir::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AIBossRevivir::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AIBossRevivir::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AIBossRevivir::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AIBossRevivir::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AIBossRevivir::EventReceived);
	Task->ReadyForActivation();
	


}


void USATORI_AIBossRevivir::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		AActor* Actor = GetAvatarActorFromActorInfo();


		ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(Actor);
		ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(Actor);

		if (Fujin)
		{
			Fujin->Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Downed.Fujin"));
			if (Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
			{
				if (!Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Fujin"))))
				{
					Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Fujin"));
					if (Fujin->Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Raijin")))
					{
						Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Fujin"));
						Fujin->Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Raijin"));
						Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos"));
						Fujin->Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos"));

					}
				}
				
			}
			Fujin->revivir();
		}
		else if (Raijin)
		{
			Raijin->Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Downed.Raijin"));
			if (Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))))
			{
				if(!Raijin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.FaseFinal.Raijin"))))
				{
					Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Raijin"));
					if (Raijin->Fujin->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Fujin")))
					{
						Raijin->Fujin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Fujin"));
						Raijin->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Raijin"));
						Raijin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos"));
						Raijin->Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.FaseFinal.Listos"));

					}
				}
				
			}
			Raijin->revivir();
		}
	}
}

void USATORI_AIBossRevivir::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
void USATORI_AIBossRevivir::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}