// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_Combo.h"
#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"

void USATORI_ANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		Character->AttackingCollision->SetGenerateOverlapEvents(true);
	}
}

void USATORI_ANS_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		Character->AttackingCollision->SetGenerateOverlapEvents(false);
	}
}