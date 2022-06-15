// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Raijin/SATORI_TPReturnSpawn.h"


#include "AI/Character/Raijin/SATORI_Raijin.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_TPReturnSpawn::USATORI_TPReturnSpawn()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_TPReturnSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());
	
	if(Raijin)
	{
		Raijin->GetMesh()->SetVisibility(false);
		Raijin->SetActorEnableCollision(false);

	}

	


	TimerDelegate = FTimerDelegate::CreateUObject(this, &USATORI_TPReturnSpawn::Teleport, Handle, ActorInfo, ActivationInfo);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);


	
	
}

void USATORI_TPReturnSpawn::Teleport(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ASATORI_Raijin* Raijin = Cast<ASATORI_Raijin>(GetAvatarActorFromActorInfo());

	if (Raijin)
	{

		Raijin->SetActorLocation(Raijin->getSpawnPos());
		Raijin->GetMesh()->SetVisibility(true);
		Raijin->SetActorEnableCollision(true);

	}


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

