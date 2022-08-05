//

#include "AI/Gas/Melee/SATORI_DashAbilityMelee.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


USATORI_DashAbilityMelee::USATORI_DashAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bDashing = false;

	bIsCreateOnRunning = GIsRunning;
}

void USATORI_DashAbilityMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bDashing = false;

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbilityMelee: Cannot get Animation Montage ... "), *GetName());
		return;
	}

	Melee = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Melee)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbilityMelee: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbilityMelee::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_DashAbilityMelee::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbilityMelee::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_DashAbilityMelee::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_DashAbilityMelee::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_DashAbilityMelee::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	//Melee->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.PlayerNonSeen"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_DashAbilityMelee::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbilityMelee::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbilityMelee::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
		ASATORI_CharacterBase* Enemy = nullptr;
		if (IsClone)
		{
			TArray< AActor* > enemigos;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character.Clone"), enemigos);

			for (AActor* Actor : enemigos)
			{
				if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
				{
					Enemy = Cast<ASATORI_CharacterBase>(Actor);
					
					
				}
			}
		}
		else
		{
			Enemy = Cast<ASATORI_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			
		}
		EnemyPosition = Enemy->GetActorLocation();

		SpawnActor();
		

		
	}
}

void USATORI_DashAbilityMelee::SpawnActor()
{
	SpawnTransform.SetLocation(EnemyPosition);

	//Missile Actor creation
	ASATORI_DashMeleeActor* DashActor = GetWorld()->SpawnActorDeferred<ASATORI_DashMeleeActor>(DashMeleeActor, SpawnTransform, GetOwningActorFromActorInfo(),
		Melee, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	DashActor->DamageGameplayEffect = DamageGameplayEffect;
	DashActor->Damage = Damage;
	DashActor->TimeToDestroy = TimeToDestroy;
	DashActor->FinishSpawning(SpawnTransform);
}

void USATORI_DashAbilityMelee::Tick(float DeltaTime)
{
	if(bDashing)
	{
		FVector Position = Melee->GetActorLocation();

		FVector NextPos = UKismetMathLibrary::VInterpTo(Position, EnemyPosition, DeltaTime, 5.f);

		Melee->SetActorLocation(NextPos);
	}

	if(Melee->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop")))
	{
		Melee->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop"));
		bDashing = false;
	}
}

bool USATORI_DashAbilityMelee::IsTickable() const
{
	return IsActive();
}

bool USATORI_DashAbilityMelee::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_DashAbilityMelee::GetStatId() const
{
	return UObject::GetStatID();
}