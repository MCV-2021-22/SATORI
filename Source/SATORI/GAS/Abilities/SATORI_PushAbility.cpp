// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SATORI_PushAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "TimerManager.h"

USATORI_PushAbility::USATORI_PushAbility ()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//Ability Tags
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Push"));
	AbilityTags.AddTag(Tag);
	ActivationOwnedTags.AddTag(Tag);

	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

	//Ability default parameters
	TimeToFinish = 1.5f;
	CastDelay = 0.2f;
	Range = 3000.0f;
	AngleRange = 60.0f;
	SphereRadius = 32.0f;

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

	//This calcs are for designing parameters for the ability
	// 
	//Calc for number of spheres to spawn to cover all range
	int NumberOfSpheresToSpawn = (FMath::Tan(FMath::DegreesToRadians(AngleRange))) * Range * 2 / (SphereRadius * 2);
	UE_LOG(LogTemp, Display, TEXT("Number of spheres: %d"), NumberOfSpheresToSpawn);

	//Calc for cone spawning
	FRotator RotationOfSphere = Character->GetActorRotation();
	RotationOfSphere.Yaw -= AngleRange / 2;
	float IncrementAngle = AngleRange / NumberOfSpheresToSpawn;

	//Spawn location
	FVector InFront = Character->GetActorForwardVector() * 100.0f;
	FTransform SphereTransform = Character->GetTransform();
	SphereTransform.AddToTranslation(InFront);

	for (int i = 0; i < NumberOfSpheresToSpawn; i++) {

		//Spawn Rotation
		RotationOfSphere.Yaw += IncrementAngle;
		SphereTransform.SetRotation(RotationOfSphere.Quaternion());
		
		ASATORI_PushActor* Push = GetWorld()->SpawnActor<ASATORI_PushActor>(
			PushActor, //Actor to Spawn
			Character->GetActorLocation() + Character->GetActorForwardVector() * 100, //Spawn location
			RotationOfSphere); //Spawn rotation
		//if(Sphere)
		//	Sphere->InitializeParameters(SphereRadius, Speed, TimeToDestroy);
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_PushAbility::OnTimerExpiredFinish);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeToFinish, false);
	
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void USATORI_PushAbility::OnTimerExpiredFinish()
{
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