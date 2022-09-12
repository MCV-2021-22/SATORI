// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SATORI_ComboSystemComponent.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class ASATORI_AICharacter;

UENUM()
enum class EComboState : uint8
{
	None UMETA(DisplayName = "NONE"),
	LightAttack_1 UMETA(DisplayName = "LightAttack 1"),
	LightAttack_2 UMETA(DisplayName = "LightAttack 2"),
	LightAttack_3 UMETA(DisplayName = "LightAttack 3"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
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

	void ApplyKnockBackTagToEnemy(ASATORI_AICharacter* Enemy);

	// Getters and Setter
	EComboState GetCurrentComboState() { return CurrentComboState; }
	void SetComboState(EComboState State);
public:
	int lightAttackCounter = 0;
	int HeavyAttackCounter = 0;

	bool isLightAttack = true;
	bool isHeavyAttack = true;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	EComboState CurrentComboState;

};
