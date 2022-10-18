// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_CanceAbility.h"
#include "SATORICharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "GAS/SATORI_AbilitySystemComponent.h"

USATORI_ANS_CanceAbility::USATORI_ANS_CanceAbility()
{

}

void USATORI_ANS_CanceAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void USATORI_ANS_CanceAbility::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void USATORI_ANS_CanceAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}