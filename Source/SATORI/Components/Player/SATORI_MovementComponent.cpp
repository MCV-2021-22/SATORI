// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_MovementComponent.h"
#include "SATORICharacter.h"

USATORI_MovementComponent::USATORI_MovementComponent()
{

}

float USATORI_MovementComponent::GetMaxSpeed() const
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	float MovementSpeedMultiplier = 1; // Character->GetMoveSpeedMultiplier();
	return Character->GetMoveSpeed() * MovementSpeedMultiplier;
}