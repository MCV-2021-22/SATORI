// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SATORI_AbilitySystemComponent.generated.h"
// https://gitlab.com/upfmcv2022/uemcvgame/-/blob/main/MCVGame/Plugins/MCVGASFramework/Source/MCVGASFramework/Public/Components/MCV_AbilitySystemComponent.h

class APawn;
class AController;

UCLASS()
class SATORI_API USATORI_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	//~ Begin UAbilitySystemComponent interface
	//virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	//~ End UAbilitySystemComponent interface

	//~ Begin UObject interface
	virtual void BeginDestroy() override;

	bool SatoriCheckIsAbilityActive(const FGameplayTagContainer* WithTags, UGameplayAbility* Ignore);
protected:
	UFUNCTION() // UFunction to be able to bind with dynamic delegate
	void OnPawnControllerChanged(APawn* Pawn, AController* NewController);
};
