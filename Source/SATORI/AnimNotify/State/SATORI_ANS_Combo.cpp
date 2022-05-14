// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_Combo.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

void USATORI_ANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// UE_LOG(LogTemp, Display, TEXT("ComboSection Begin"));

	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		USATORI_ComboSystemComponent* ComboSystem = PlayerCharacter->GetComboSystemComponent();
		if (ComboSystem)
		{
			UE_LOG(LogTemp, Display, TEXT("ComboSection Begin"));
			if (ComboState == EComboState::SafeAttack)
			{
				ComboSystem->lightAttackCounter += 1;
			}
		}
	}
	/*

	if (ComboState == EComboState::ResetAttack)
	{
		ComboSystem->lightAttackCounter = 0;
		ComboSystem->HeavyAttackCounter = 0;
	}

	else if (ComboState == EComboState::SafeAttack)
	{
		ComboSystem->lightAttackCounter += 1;
	}*/
}

void USATORI_ANS_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	//UE_LOG(LogTemp, Display, TEXT("ComboSection End"));

	//ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(MeshComp->GetOwner());
	//USATORI_ComboSystemComponent* ComboSystem = PlayerCharacter->GetComboSystemComponent();

	//if (ComboState == EComboResetState::ResetAttack)
	//{
	//	// Reset only Attack and Counter
	//}

	//else if (ComboState == EComboResetState::SafeAttack)
	//{
	//	// Reset Only Attack
	//}

	//ComboSystem->ResetAllAttribute();
}