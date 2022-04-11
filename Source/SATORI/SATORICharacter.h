// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Character/Mask/SATORI_AbilityMask.h"
#include "SATORICharacter.generated.h"

class USATORI_AbilityDataAsset;
class UGameplayEffect;
class USATORI_AttributeSet;
class USATORI_AbilitySystemComponent;

UCLASS(config=Game)
class ASATORICharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASATORICharacter();

	virtual void PossessedBy(AController* NewController) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Character Default Abilities Asset (Contain List of Player Abilities)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Data")
	USATORI_AbilityDataAsset* DefaultAbilities;

	// Default attributes for a character for initializing
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY()
	SATORIMaskType MaskType = SATORIMaskType::NONE;
public:

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Gettes for attributes
	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetDefense() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	int32 GetCharacterLevel() const;
protected:

	void ApplyDefaultAbilities();
	void GrantAbilityToPlayer(FGameplayAbilitySpec Ability);
	void InitializePassiveAttributes();

	virtual void SetHealth(float Health);
private:
	// The core ActorComponent for interfacing with the GameplayAbilities System
	TWeakObjectPtr<USATORI_AbilitySystemComponent> AbilitySystemComponent;
	
	// USATORI_AttributeSet from the PlayerState 
	TWeakObjectPtr<USATORI_AttributeSet> AttributeSetBase;
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// SetupPlayerInputComponent
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
};

