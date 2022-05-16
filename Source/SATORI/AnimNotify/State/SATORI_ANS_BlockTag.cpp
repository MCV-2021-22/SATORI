// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_BlockTag.h"
#include "SATORICharacter.h"

USATORI_ANS_BlockTag::USATORI_ANS_BlockTag()
{
	bIsNativeBranchingPoint = true;
}

void USATORI_ANS_BlockTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (!MeshComp || !TagToBlock.IsValid())
	{
		return;
	}

	if (ASATORICharacter* Character = MeshComp->GetOwner<ASATORICharacter>()) 
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(TagToBlock);
		Character->BlockGameplayTag(TagContainer);
	}
}

void USATORI_ANS_BlockTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (!MeshComp || !TagToBlock.IsValid())
	{
		return;
	}

	if (ASATORICharacter* Character = MeshComp->GetOwner<ASATORICharacter>()) {
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(TagToBlock);
		Character->UnBlockGameplayTag(TagContainer);
	}
}
