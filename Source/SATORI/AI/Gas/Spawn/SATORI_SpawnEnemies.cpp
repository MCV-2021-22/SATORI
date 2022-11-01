// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Spawn/SATORI_SpawnEnemies.h"
#include "SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

USATORI_SpawnEnemies::USATORI_SpawnEnemies()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_SpawnEnemies::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_SpawnEnemies: Cannot get Animation Montage ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Spawner = Cast<ASATORI_Spawner>(GetAvatarActorFromActorInfo());
	if (!Spawner)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_SpawnEnemies: Cannot Cast Character ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_SpawnEnemies: Failed commit ability ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->EventReceived.AddDynamic(this, &USATORI_SpawnEnemies::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_SpawnEnemies::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_SpawnEnemies::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_SpawnEnemies::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_SpawnEnemies::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == SpawnTag)
	{
		if (Spawner)
		{
			Spawner->spawning = true;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FRotator RotationOfIA = GetAvatarActorFromActorInfo()->GetActorRotation();
			ASATORI_Spawned* EnemySpawned = GetWorld()->SpawnActor<ASATORI_Spawned>(EnemyToSpawn,
				GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector() * 100,
				RotationOfIA, SpawnParams);

			if(EnemySpawned)
			{
				EnemySpawned->SpawnDefaultController();
				EnemySpawned->SetMySpawn(Spawner);
			}
			Spawner->AddNumEnemies(1);

			Spawner->ResetSpawnTime();
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}