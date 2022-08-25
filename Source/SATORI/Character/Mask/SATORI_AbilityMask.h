// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SATORI_AbilityMask.generated.h"

UENUM(BlueprintType)
enum class SATORIMaskType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	Aka UMETA(DisplayName = "Aka"),
	Ao UMETA(DisplayName = "Ao"),
	Midori UMETA(DisplayName = "Midori"),
};

class UGameplayEffect;

/*
* SATORI Mask Class contain 3 Mask Type
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_AbilityMask : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_AbilityMask();

	void GrantedMaskEffects(SATORIMaskType MaskType);

	// Helper function to choose which GE to Apply after player choose the mask
	TSubclassOf<UGameplayEffect> ChooseMaskEffectoToApply(SATORIMaskType MaskType);

	FActiveGameplayEffectHandle GetCurrentActiveGEHandle();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Mask Passvie GE to apply to player
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> AkaGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> AoGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> MidoriGameplayEffect;
	//TMap<SATORIMaskType, UGameplayEffect> MaskEffects;

private:
	FActiveGameplayEffectHandle ActiveGEHandle;
};
