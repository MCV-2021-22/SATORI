// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AbilityMask.h"
#include "SATORI/SATORICharacter.h"
#include "SATORI/Character/SATORI_PlayerState.h"

// Sets default values for this component's properties
USATORI_AbilityMask::USATORI_AbilityMask()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USATORI_AbilityMask::BeginPlay()
{
	Super::BeginPlay();

}

void USATORI_AbilityMask::GrantedMaskEffects()
{
	/*for (TSubclassOf<UGameplayEffect>& GameplayEffect : MaskPassive)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
				AbilitySystemComponent.Get());
		}
	}*/
}

