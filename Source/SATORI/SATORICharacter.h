// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Character/Mask/SATORI_AbilityMask.h"
#include "Character/SATORI_CharacterBase.h"
#include "SATORICharacter.generated.h"

class USATORI_AbilityDataAsset;
class UGameplayEffect;
class USATORI_AttributeSet;
class USATORI_AbilitySystemComponent;
class USATORI_AbilityMask;
class USATORI_ANS_JumpSection;

UCLASS(config=Game)
class ASATORICharacter : public ASATORI_CharacterBase
{
	GENERATED_BODY()

public:
	ASATORICharacter();

	// For player controlled characters where the ASC lives on the Pawn
	virtual void PossessedBy(AController* NewController) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Default attributes for a character for initializing
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|Component")
	USATORI_AbilityMask* SATORIAbilityMaskComponent;

	UPROPERTY()
	SATORIMaskType MaskType = SATORIMaskType::NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int AbilityToChoose = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTagContainer GameplayTags;

public:

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
	float GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "Player|Attributes")
	int32 GetCharacterLevel() const;

	void SetComboJumpSection(USATORI_ANS_JumpSection* JumpSection);

	// Combos 
	UFUNCTION(BlueprintCallable)
	bool AttackJumpSectionCombo();

	UFUNCTION(BlueprintCallable)
	bool PlayerActiveAbilityWithTag(FGameplayTag TagName);

	void AddGameplayTag(const FGameplayTag& TagToAdd);
	void RemoveGameplayTag(const FGameplayTag& TagToRemove);
	void BlockGameplayTag(const FGameplayTagContainer& TagsToBlock);
	void UnBlockGameplayTag(const FGameplayTagContainer& TagsToBlock);

	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;
	bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const;

	bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const;

	bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const;

	// Getters for Components
	FORCEINLINE class USATORI_StatsComponent* GetStatsComponent() const { return StatsComponent; }
	class USATORI_ComboSystemComponent* GetComboSystemComponent() const { return ComboSystemComponent; }
protected:

	// Initialization for player abilities
	void ApplyDefaultAbilities();
	// Helper function Granting a GameplayAbility to an ASC adds it to the ASC's list of ActivatableAbilities allowing it to activate the GameplayAbility
	void GrantAbilityToPlayer(FGameplayAbilitySpec Ability);
	void InitializePassiveAttributes();

	virtual void SetHealth(float Health);

protected:

	// The core ActorComponent for interfacing with the GameplayAbilities System
	//TWeakObjectPtr<USATORI_AbilitySystemComponent> AbilitySystemComponent;
	
	// USATORI_AttributeSet from the PlayerState 
	TWeakObjectPtr<USATORI_AttributeSet> AttributeSetBase;

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USATORI_StatsComponent* StatsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USATORI_ComboSystemComponent* ComboSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USATORI_GameplayAbilityComponent* PlayerGameplayAbilityComponent;

	// Anim Notify Section
	UPROPERTY()
	USATORI_ANS_JumpSection* JumpSectionNS = nullptr;

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

