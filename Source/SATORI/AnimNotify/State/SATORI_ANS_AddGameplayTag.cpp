// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_AddGameplayTag.h"
#include "SATORICharacter.h"
#include "Character/SATORI_CharacterBase.h"

USATORI_ANS_AddGameplayTag::USATORI_ANS_AddGameplayTag()
{
	bIsNativeBranchingPoint = true;
}

void USATORI_ANS_AddGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (!MeshComp || !TagToAdd.IsValid()) 
	{
		return;
	}

	if (ASATORI_CharacterBase* Character = MeshComp->GetOwner<ASATORI_CharacterBase>()) {
		Character->GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	}
}

void USATORI_ANS_AddGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (!MeshComp || !TagToAdd.IsValid())
	{
		return;
	}

	if (ASATORI_CharacterBase* Character = MeshComp->GetOwner<ASATORI_CharacterBase>()) {
		Character->GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToAdd);
	}

}
