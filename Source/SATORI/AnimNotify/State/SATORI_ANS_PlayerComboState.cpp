// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/State/SATORI_ANS_PlayerComboState.h"
#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Actors/AbilitiesActors/SATORI_Push360Actor.h"

USATORI_ANS_PlayerComboState::USATORI_ANS_PlayerComboState()
{

}

void USATORI_ANS_PlayerComboState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ASATORICharacter* Character = Cast<ASATORICharacter>(MeshComp->GetOwner());
	if (Character)
	{
		USATORI_ComboSystemComponent* ComboComponent = Character->GetComboSystemComponent();
		if (ComboComponent)
		{
			ComboComponent->SetComboState(ComboState);
			// print enum to string
			const TEnumAsByte<EComboState> EComboStateEnum = ComboState;
			FString EnumAsString = UEnum::GetValueAsString(EComboStateEnum.GetValue());
			//UE_LOG(LogTemp, Warning, TEXT("My Current combo state is : %s"), *EnumAsString);			
		}
	}
}

void USATORI_ANS_PlayerComboState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ASATORICharacter* Character = Cast<ASATORICharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->WeaponDamage = Character->WeaponSavedDamage;

		USATORI_ComboSystemComponent* ComboComponent = Character->GetComboSystemComponent();
		if (ComboComponent)
		{
			ComboComponent->SetComboState(EComboState::None);
			//UE_LOG(LogTemp, Warning, TEXT("Reset my current combo state"));
		}
	}
}