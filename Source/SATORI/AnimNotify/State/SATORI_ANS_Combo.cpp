// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_Combo.h"
#include "SATORICharacter.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "Components/CapsuleComponent.h"

void USATORI_ANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);


	ASATORI_CharacterBase* PlayerCharacter = Cast<ASATORI_CharacterBase>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->AttackingCollision->SetGenerateOverlapEvents(true);
		UE_LOG(LogTemp, Display, TEXT("ComboSection Begin"));

		//USATORI_ComboSystemComponent* ComboSystem = PlayerCharacter->GetComboSystemComponent();
		//if (ComboSystem)
		//{
		//	UE_LOG(LogTemp, Display, TEXT("ComboSection Begin"));
		//	/*if (ComboState == EComboState::SafeAttack)
		//	{
		//		ComboSystem->lightAttackCounter += 1;
		//	}*/
		//	ComboSystem->isLightAttack = false;
		//	//ComboSystem->AttackingCollision->Activate(true);
		//}
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


	ASATORI_CharacterBase* PlayerCharacter = Cast<ASATORI_CharacterBase>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->AttackingCollision->SetGenerateOverlapEvents(false);
		UE_LOG(LogTemp, Display, TEXT("ComboSection End"));

		//USATORI_ComboSystemComponent* ComboSystem = PlayerCharacter->GetComboSystemComponent();
		//if (ComboSystem)
		//{
		//	UE_LOG(LogTemp, Display, TEXT("ComboSection Begin"));
		//	/*if (ComboState == EComboState::SafeAttack)
		//	{
		//		ComboSystem->lightAttackCounter += 1;
		//	}*/
		//	ComboSystem->isLightAttack = true;
		//	//ComboSystem->AttackingCollision->Activate(false);
		//}
	}
}