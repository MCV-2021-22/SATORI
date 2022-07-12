// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AbilityMask.h"
#include "SATORI/SATORICharacter.h"
#include "SATORI/Character/SATORI_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"

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

void USATORI_AbilityMask::GrantedMaskEffects(SATORIMaskType MaskType)
{
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			TSubclassOf<UGameplayEffect> GameplayEffect = ChooseMaskEffectoToApply(MaskType);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,
				PlayerCharacter->GetCharacterLevel(), EffectContext);

			if (NewHandle.IsValid())
			{
				ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
					AbilitySystemComponent);
			}
		}
	}
}

FActiveGameplayEffectHandle USATORI_AbilityMask::GetCurrentActiveGEHandle()
{
	return ActiveGEHandle;
}

TSubclassOf<UGameplayEffect> USATORI_AbilityMask::ChooseMaskEffectoToApply(SATORIMaskType MaskType)
{
	TSubclassOf<UGameplayEffect> Effect;

	switch (MaskType)
	{
	case SATORIMaskType::NONE:
		break;
	case SATORIMaskType::Aka:
		Effect = AkaGameplayEffect;
		break;
	case SATORIMaskType::Ao:
		Effect = AoGameplayEffect;
		break;
	case SATORIMaskType::Midori:
		Effect = MidoriGameplayEffect;
		break;
	}

	return Effect;
}
