// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Character/Mask/SATORI_AbilityMask.h"
#include "Components/Player/SATORI_TargetSystemComponent.h"
#include "Character/SATORI_CharacterBase.h"
#include "FMODBlueprintStatics.h"
#include "SATORICharacter.generated.h"

class USATORI_AbilityDataAsset;
class USATORI_GameplayEffect;
class UGameplayEffect;
class USATORI_AbilitySystemComponent;
class USATORI_AbilityMask;
class USATORI_ANS_JumpSection;
class USATORI_GameplayAbility;
class USkeletalMeshComponent;
class UCapsuleComponent;
class USATORI_InteractComponent;
class USATORI_GameplayAbilityComponent;
class USATORI_PlayerCameraShake;

UCLASS(config=Game)
class ASATORICharacter : public ASATORI_CharacterBase
{
	GENERATED_BODY()

public:

	ASATORICharacter();

	// For player controlled characters where the ASC lives on the Pawn
	virtual void PossessedBy(AController* NewController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFMODEvent* HitPlayer = nullptr;;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName;

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

	// Death Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Death|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY()
	SATORIMaskType MaskType = SATORIMaskType::NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int AbilityToChoose = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool isPlayerDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* HandComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponDamage = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponSavedDamage = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bMultipleHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName SocketName = "BoSocket";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY()
	USATORI_GameplayEffect* ManaRecoverGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> StunGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AnimactionPlayRater = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Setting")
	float VisibleAttackLength = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Setting")
	float VisibleAttackAngle = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack Setting")
	int AttackRange = 5;

	void SetComboJumpSection(USATORI_ANS_JumpSection* JumpSection);

	// Combos 
	UFUNCTION(BlueprintCallable)
	bool AttackJumpSectionCombo();

	// Ray Cast
	UFUNCTION(BlueprintCallable)
	bool DoParryBlock();

	UFUNCTION(BlueprintCallable)
	bool IsEnemyInFrontOfAngle();

	UFUNCTION()
	void SetCharacterMask(SATORIMaskType GrantedMaskType);

	UFUNCTION(BlueprintCallable)
	bool PlayerActiveAbilityWithTag(FGameplayTag TagName);

	UFUNCTION(BlueprintCallable)
	void ApplyGameplayeEffectToPlayerWithParam(TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION(BlueprintCallable)
	void PlayerSenseOfBlow(float DilationTime = 0.8f, float WaitTime = 0.5f);

	// Player Camera Shake
	UFUNCTION(BlueprintCallable)
	void PlayerCameraShake();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowDeathWidget();

	// Remove Player Mask Effecr
	UFUNCTION()
	void RemoveMaskGameplayEffect();

	virtual void CharacterDeath() override;
	
	virtual void RemoveCharacterAbilities() override;

	virtual void Tick(float DeltaSeconds) override;

	bool acti_rayo = false;

	bool singlerayo = false;

	bool activoRayo = false;

	float maxTimeRayo = 1.0f;

	float timeRayo = 0.0f;

	// Reset All character Datas
	UFUNCTION()
	void ResetCharacterDatas();

	// Cancel Ability With Tag
	UFUNCTION()
	bool PlayerCancelAbilityWithTag(FGameplayTagContainer& GameplayTagContainer);

	// Getters
	bool GetIsAbilityUpgrated() { return IsAbilityUpgrated; }
	void SetIsAbilityUpgrated(bool Value) { IsAbilityUpgrated = Value; }
	// Getters for Components
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class USATORI_StatsComponent* GetStatsComponent() const { return StatsComponent; }

	UFUNCTION(BlueprintCallable)
	class USATORI_ComboSystemComponent* GetComboSystemComponent() const { return ComboSystemComponent; }

	UFUNCTION(BlueprintCallable)
	class USATORI_TargetSystemComponent* GetTargetSystemComponent() const { return TargetSystemComponent; }

	UFUNCTION(BlueprintCallable)
	USceneComponent* GetHandComponent() const { return HandComponent; }

	UFUNCTION(BlueprintCallable)
	USATORI_GameplayAbilityComponent* GetPlayerAbilityComponent() const { return PlayerGameplayAbilityComponent; }
protected:

	// Initialization for player abilities
	UFUNCTION()
	void ApplyDefaultAbilities();

	// Helper function Granting a GameplayAbility to an ASC adds it to the ASC's list of ActivatableAbilities allowing it to activate the GameplayAbility
	UFUNCTION()
	void GrantAbilityToPlayer(FGameplayAbilitySpec Ability);

	UFUNCTION()
	void InitializePassiveAttributes();

	// Check if the enemy is in front
	UFUNCTION()
	bool IsEnemyInFront(const FVector StartPosition, const FVector EndPosition, FHitResult& LocalHitResult, int RotationSize = 1);

	TWeakObjectPtr<AActor> FindNearestEnemy(TArray<TWeakObjectPtr<AActor>> Actors);

	// Parry
	UFUNCTION()
	bool DoParryBlockAllEnemies();

	UFUNCTION()
	bool DoParryBlockOneEnemies();

	// Set Anim Instance class 
	UFUNCTION()
	void InitializeAnimIntance();

protected:

	// The core ActorComponent for interfacing with the GameplayAbilities System
	//TWeakObjectPtr<USATORI_AbilitySystemComponent> AbilitySystemComponent;
	
	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USATORI_StatsComponent* StatsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USATORI_ComboSystemComponent* ComboSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USATORI_InteractComponent* InteractComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USATORI_GameplayAbilityComponent* PlayerGameplayAbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAbilityUpgrated = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* ParryCollision;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USATORI_TargetSystemComponent* TargetSystemComponent;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<USATORI_PlayerCameraShake> CameraShake;
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

	void OnInteract();

	void OnDash();
	void OnDashReleases();

	// Cheat
	void ActivatePlayerCheat();

	// Weapon Overlap
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlapEnd(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Parry Overlap
	UFUNCTION()
		void OnParryOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnParryOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
//Cheats
public:

	UFUNCTION(Exec)
	void SetGodMode();

	UFUNCTION(Exec)
	void RestartStats();

	UFUNCTION(Exec)
	void RemoveAllAbilities();

	UFUNCTION(Exec)
	void KillAllEnemies();

	UFUNCTION(Exec)
	void KillPlayer();

};

