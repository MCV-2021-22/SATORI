//

#include "AI/Gas/Melee/SATORI_AI_DashAbilityMelee.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

USATORI_AI_DashAbilityMelee::USATORI_AI_DashAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bDashing = false;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_AI_DashAbilityMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	bDashing = false;

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_AI_DashAbilityMelee: Cannot get Animation Montage ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Melee = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (!Melee)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbilityMelee: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid() || !TagStartDash.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AI_DashAbilityMelee: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AI_DashAbilityMelee::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_AI_DashAbilityMelee::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_AI_DashAbilityMelee::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_AI_DashAbilityMelee::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_AI_DashAbilityMelee::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagEndAbility)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (EventTag == TagStartDash)
	{
		bDashing = true;
	}

	if (EventTag == TagSpawnAbility)
	{
		UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetAvatarActorFromActorInfo());
		
		if (IsValid(Blackboard))
		{
			FName Clone = "Clone";
			FName Player = "Target";
			
			ASATORI_CharacterBase* Target = nullptr;
			Target = Cast<ASATORI_CharacterBase>(Blackboard->GetValueAsObject(Clone));

			//Check clone
			if (IsValid(Target))
			{
				EnemyPosition = Target->GetActorLocation();
				SpawnActor();
			}
			//If clone fails then checks for player
			else
			{
				Target = Cast<ASATORI_CharacterBase>(Blackboard->GetValueAsObject(Player));

				if (IsValid(Target))
				{
					EnemyPosition = Target->GetActorLocation();
					SpawnActor();
				}
			}
		}
	}
}

void USATORI_AI_DashAbilityMelee::SpawnActor()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(EnemyPosition);

	//Decal creation
	ASATORI_DashMeleeActor* DashActor = GetWorld()->SpawnActorDeferred<ASATORI_DashMeleeActor>(DashMeleeActor, SpawnTransform, GetOwningActorFromActorInfo(), Melee, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	DashActor->DamageGameplayEffect = DamageGameplayEffect;
	DashActor->Damage = Damage;
	DashActor->TimeToDestroy = TimeToDestroy;
	DashActor->FinishSpawning(SpawnTransform);
}

void USATORI_AI_DashAbilityMelee::Tick(float DeltaTime)
{
	if(bDashing)
	{
		FVector Position = Melee->GetActorLocation();
		FVector NextPos = UKismetMathLibrary::VInterpTo(Position, EnemyPosition, DeltaTime, DashSpeed);
		Melee->SetActorLocation(NextPos);
	}

	if(Melee->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop")))
	{
		Melee->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop"));
		bDashing = false;
	}
}

bool USATORI_AI_DashAbilityMelee::IsTickable() const
{
	return IsActive();
}

bool USATORI_AI_DashAbilityMelee::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_AI_DashAbilityMelee::GetStatId() const
{
	return UObject::GetStatID();
}