// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_ComboMelee.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "Components/CapsuleComponent.h"

void USATORI_ANS_ComboMelee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// UE_LOG(LogTemp, Display, TEXT("ComboSection Begin"));

	ASATORI_AICharacter* PlayerCharacter = Cast<ASATORI_AICharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->AttackingCollision->SetGenerateOverlapEvents(true);
		PlayerCharacter->AttackingCollision2->SetGenerateOverlapEvents(true);
	}
}

void USATORI_ANS_ComboMelee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UE_LOG(LogTemp, Display, TEXT("ComboSection End"));

	ASATORI_AICharacter* PlayerCharacter = Cast<ASATORI_AICharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->AttackingCollision->SetGenerateOverlapEvents(false);
		PlayerCharacter->AttackingCollision2->SetGenerateOverlapEvents(false);
	}
}