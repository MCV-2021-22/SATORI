// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Spawn/SATORI_SpawnEnemies.h"

#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_SpawnEnemies::USATORI_SpawnEnemies()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_SpawnEnemies::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FRotator RotationOfIA = ActorInfo->AvatarActor->GetActorRotation();
	ASATORI_Spawned* EnemySpawned = GetWorld()->SpawnActor<ASATORI_Spawned>(EnemyToSpawn,
		ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 100,
		RotationOfIA, SpawnParams);


	ASATORI_Spawner* Spawner = Cast<ASATORI_Spawner>(ActorInfo->AvatarActor);

	EnemySpawned->SpawnDefaultController();
	EnemySpawned->SetMySpawn(Spawner);
	
	Spawner->AddNumEnemies(1);

	Spawner->ResetSpawnTime();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}