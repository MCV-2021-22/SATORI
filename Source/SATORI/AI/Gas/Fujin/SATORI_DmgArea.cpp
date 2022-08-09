// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Gas/Fujin/SATORI_DmgArea.h"

#include "SATORICharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

USATORI_DmgArea::USATORI_DmgArea()
{

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void USATORI_DmgArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{


	int a = 2;


	
}



