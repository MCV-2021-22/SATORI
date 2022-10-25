// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SATORI_ANS_CanceAbility.generated.h"

class ASATORICharacter;
class UAnimMontage;

UCLASS()
class SATORI_API USATORI_ANS_CanceAbility : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	USATORI_ANS_CanceAbility();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CancelGameplayTag")
	FGameplayTag TagToCancelAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* MontageToPlay = nullptr;
};
