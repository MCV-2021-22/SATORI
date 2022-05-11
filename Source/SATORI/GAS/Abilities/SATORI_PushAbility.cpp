// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_PushAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Kismet/KismetMathLibrary.h"

USATORI_PushAbility::USATORI_PushAbility ()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	FGameplayTag PushTag = FGameplayTag::RequestGameplayTag(FName("Ability.Push"));
	AbilityTags.AddTag(PushTag);
	ActivationOwnedTags.AddTag(PushTag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	CastDelay = 0.2f;
	Speed = 300.0f;
	Range = 500.0f;
	AngleRange = 30.0f;
	SphereRadius = 64.0f;
	Damage = 12.0f;

}

void USATORI_PushAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
	ASATORICharacter* Character = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());

	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	//Calc for number of spheres to spawn to cover all range
	int NumberOfSpheresToSpawn = (FMath::Tan(FMath::DegreesToRadians(AngleRange))) * Range * 2 / (SphereRadius * 2);
	UE_LOG(LogTemp, Display, TEXT("Number of spheres: %d"), NumberOfSpheresToSpawn);

	//Calc for cone spawning
	FRotator RotationOfSphere = Character->GetActorRotation();
	RotationOfSphere.Yaw -= AngleRange / 2;
	float IncrementAngle = AngleRange / NumberOfSpheresToSpawn;
	
	float TimeToDestroy = Range / Speed; //Time to destroy sphere

	for (int i = 0; i < NumberOfSpheresToSpawn; i++) {

		RotationOfSphere.Yaw += IncrementAngle;

		ASATORI_PushSphere* Sphere = GetWorld()->SpawnActor<ASATORI_PushSphere>(
			PushSphereClass, //Actor to Spawn
			Character->GetActorLocation() + Character->GetActorForwardVector() * 100, //Spawn location
			RotationOfSphere); //Spawn rotation

		Sphere->delayedDestroy(TimeToDestroy);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void USATORI_PushAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_PushAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_PushAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

}