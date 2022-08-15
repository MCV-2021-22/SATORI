// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_ComboBothWeapons.h"
#include "AI/Character/Melee/SATORI_Melee.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "Components/CapsuleComponent.h"

void USATORI_ANS_ComboBothWeapons::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ASATORI_Melee* MeleeCharacter = Cast<ASATORI_Melee>(MeshComp->GetOwner());
	if (MeleeCharacter)
	{
		MeleeCharacter->AttackingCollision->SetGenerateOverlapEvents(true);
		MeleeCharacter->AttackingCollisionLeft->SetGenerateOverlapEvents(true);
	}
}

void USATORI_ANS_ComboBothWeapons::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ASATORI_Melee* MeleeCharacter = Cast<ASATORI_Melee>(MeshComp->GetOwner());
	if (MeleeCharacter)
	{
		MeleeCharacter->AttackingCollision->SetGenerateOverlapEvents(false);
		MeleeCharacter->AttackingCollisionLeft->SetGenerateOverlapEvents(false);
	}
}