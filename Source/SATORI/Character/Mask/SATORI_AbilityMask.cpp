// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_AbilityMask.h"
#include "SATORI/SATORICharacter.h"
#include "SATORI/Character/SATORI_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Engine/Texture2D.h"
#include "GameplayFramework/SATORI_GameInstance.h"

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

			// Broadcast
			PortrailImageChange.Broadcast(CurrentPortrailImage);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,
				PlayerCharacter->GetCharacterLevel(), EffectContext);

			if (NewHandle.IsValid())
			{
				ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
					AbilitySystemComponent);
			}
		}
	}

	USATORI_GameInstance* GameInstanceRef = 
		Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstanceRef)
	{
		GameInstanceRef->MaskType = MaskType;
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
		if (DefaultImage)
		{
			CurrentPortrailImage = DefaultImage;
		}
		break;
	case SATORIMaskType::Aka:
		Effect = AkaGameplayEffect;
		if (AkaImage)
		{
			CurrentPortrailImage = AkaImage;
		}
		break;
	case SATORIMaskType::Ao:
		Effect = AoGameplayEffect;
		if (AoImage)
		{
			CurrentPortrailImage = AoImage;
		}
		break;
	case SATORIMaskType::Midori:
		Effect = MidoriGameplayEffect;
		if (MidoriImage)
		{
			CurrentPortrailImage = MidoriImage;
		}
		break;
	}

	return Effect;
}

UTexture2D* USATORI_AbilityMask::SelectMaskToPortrail(SATORIMaskType MaskType)
{
	if (MaskType == SATORIMaskType::NONE)
	{
		return CurrentPortrailImage;
	}
	else if (MaskType == SATORIMaskType::Aka && AkaImage)
	{
		return AkaImage;
	}
	else if (MaskType == SATORIMaskType::Ao && AoImage)
	{
		return AoImage;
	}
	else if (MaskType == SATORIMaskType::Midori && MidoriImage)
	{
		return MidoriImage;
	}
	return CurrentPortrailImage;
}