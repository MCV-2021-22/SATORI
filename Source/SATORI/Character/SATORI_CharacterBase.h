// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "SATORI_CharacterBase.generated.h"

class USATORI_AbilitySystemComponent;

UCLASS()
class SATORI_API ASATORI_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASATORI_CharacterBase();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The core ActorComponent for interfacing with the GameplayAbilities System
	TWeakObjectPtr<USATORI_AbilitySystemComponent> AbilitySystemComponent;
};
