// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_JumpSection.h"
#include "SATORICharacter.h"

void USATORI_ANS_JumpSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UE_LOG(LogTemp, Display, TEXT("UJumpSectionNS Begin"));

	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(MeshComp->GetOwner());

	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->SetComboJumpSection(this);
}

void USATORI_ANS_JumpSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UE_LOG(LogTemp, Display, TEXT("UJumpSectionNS End"));

	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(MeshComp->GetOwner());

	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->SetComboJumpSection(nullptr);
}

