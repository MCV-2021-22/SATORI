// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SATORI_ANS_Combo.generated.h"

UENUM()
enum class EComboState : uint8
{
	None UMETA(DisplayName = "NONE"),
	ResetAttack UMETA(DisplayName = "ResetAttack"),
	SafeAttack UMETA(DisplayName = "SafeAttack"),
};

UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (ShowWorldContextPin, DisplayName = "Combo Start"))
class SATORI_API USATORI_ANS_Combo : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EComboState> ComboState;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
