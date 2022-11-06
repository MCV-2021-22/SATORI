// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "SATORI_AbilityMask.generated.h"

class UImage;
class UTexture2D;

UENUM(BlueprintType)
enum class SATORIMaskType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	Aka UMETA(DisplayName = "Aka"),
	Ao UMETA(DisplayName = "Ao"),
	Midori UMETA(DisplayName = "Midori"),
};

USTRUCT(BlueprintType)
struct FSATORI_PlayerPortrailImage
{	
	GENERATED_BODY()

	UTexture2D* DefaultImage = nullptr;
	UTexture2D* AkaImage = nullptr;
	UTexture2D* AoImage = nullptr;
	UTexture2D* MidoriImage = nullptr;
};

class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIPortrailImageChanges, UTexture2D*, ImageData);

/*
* SATORI Mask Class contain 3 Mask Type
*/
UCLASS(BlueprintType)
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

	UPROPERTY(BlueprintAssignable)
	FSATORIPortrailImageChanges PortrailImageChange;

	void SelectMaskToPortrail(SATORIMaskType MaskType);
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> MidoriHealthGameplayEffect;
	//TMap<SATORIMaskType, UGameplayEffect> MaskEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PortrailImage")
	UTexture2D* DefaultImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PortrailImage")
	UTexture2D* AkaImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PortrailImage")
	UTexture2D* AoImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PortrailImage")
	UTexture2D* MidoriImage = nullptr;
private:
	FActiveGameplayEffectHandle ActiveGEHandle;

	UTexture2D* CurrentPortrailImage;
};
