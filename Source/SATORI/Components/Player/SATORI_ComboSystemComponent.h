// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SATORI_ComboSystemComponent.generated.h"

class USphereComponent;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class SATORIComboType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	LightCombo_1 UMETA(DisplayName = "LightCombo 1"),
	LightCombo_2 UMETA(DisplayName = "LightCombo 2"),
	LightCombo_3 UMETA(DisplayName = "LightCombo 3"),
	HeavyCombo_1 UMETA(DisplayName = "HeavyCombo 1")
};

// UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
UCLASS(BlueprintType)
class SATORI_API USATORI_ComboSystemComponent : public UActorComponent
{
	GENERATED_BODY() 

public:	
	// Sets default values for this component's properties
	USATORI_ComboSystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	TArray<UAnimMontage*> LightAttackMontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	TArray<UAnimMontage*> HeavyAttackMontageToPlay;

	UFUNCTION(BlueprintCallable)
	void LigthAttack();

	UFUNCTION(BlueprintCallable)
	void HeavyAttack();

	UFUNCTION(BlueprintCallable)
	bool CanComboAttack();

	void ResetAllAttribute();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USphereComponent* AttackingCollision;

	UFUNCTION()
	void SetComboType(SATORIComboType Type);
public:
	int lightAttackCounter = 0;
	int HeavyAttackCounter = 0;

	bool isLightAttack = true;
	bool isHeavyAttack = true;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	UPROPERTY()
	SATORIComboType ComboType;
};
