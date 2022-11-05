// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/SATORI_TargetSystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Character/SATORI_CharacterBase.h"
#include "GameplayTags.h"
#include "GAS\SATORI_GameplayAbility.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "SATORI_AICharacter.generated.h"


class UGameplayEffect;
class USATORI_AttributeSet;
class USATORI_AbilitySystemComponent;
class USATORI_GameplayAbility;
class USATORI_AbilityDataAsset;
class UBehaviorTree;
class UPawnSensingComponent;
class USkeletalMeshComponent;
class UWidgetComponent;
class USATORI_EnemyHealthBar;
class USATORI_EnemyStatComponent;
class ASATORI_CoinRewardActor;
class AAIController;

UENUM(BlueprintType)
enum class SATORIEnemyType : uint8
{
	None UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Boss UMETA(DisplayName = "Boss"),
};

UENUM(BlueprintType)
enum class SATORIEnemyImpactType : uint8
{
	None UMETA(DisplayName = "None"),
	LightAttack1 UMETA(DisplayName = "LightAttack1"),
	LightAttack2 UMETA(DisplayName = "LightAttack2"),
	LightAttack3 UMETA(DisplayName = "LightAttack3"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
};

UCLASS()
class SATORI_API ASATORI_AICharacter : public ASATORI_CharacterBase, public ISATORI_TargetSystemInterface
{
	GENERATED_BODY()

public:

	ASATORI_AICharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness)
	UPawnSensingComponent* PawnSensor;

	UPROPERTY()
	USATORI_AttributeSet* AttributeSet;

	// Character Default Abilities Asset (Contain List of Player Abilities)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GAS")
	USATORI_AbilityDataAsset* DefaultAbilities;
	
	// Default attributes for a character for initializing
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCapsuleComponent* AttackingCollision2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	USATORI_EnemyStatComponent* EnemyStatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	TSubclassOf<UGameplayEffect> GoldRewardGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> CountGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> HitGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> HeavyHitGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn")
	float DesiredZPosition;

	//Ray Cast
	// Check if the player is in front of the enemy and can do the parry
	UFUNCTION(BlueprintCallable)
	bool CheckPlayerWithRayCast();	

	bool GetIsInFront() const { return isInFrontPlayer; }
	
	//Draw Debug
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool bDrawDebug = false;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// Initialize AI Attributes from GE
	void InitializeAttributes();

	// Adding Abilities to AI
	virtual void AddAICharacterAbilities();

	void GrantAbility(FGameplayAbilitySpec Ability);

	bool bursting = false;
	float time_burst = 0.0f;
	float dmg_burst = 0.0f;

public:

	bool canDestroy = false;
	//Deathchecking
	UFUNCTION(BlueprintCallable)
	void CheckDamage(float Damage);

	//Range
	float GetAttackDistance();
	float GetMaxRange();

	//Health UI
	UFUNCTION(BlueprintCallable)
	void HealthBarProjection(UWidgetComponent* HealthBar, float ViewDistance, float RangeA, float RangeB);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<USATORI_EnemyHealthBar> HealthBarUI;

	// Spawn Coin Actor
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CoinActor")
	TSubclassOf<ASATORI_CoinRewardActor> SpawnCoinActor;

	UFUNCTION(BlueprintCallable)
	void SpawnCointActorAfterDeath();

	virtual void CharacterDeath() override;

	// Check Impact Received
	UFUNCTION()
	void CheckImpactReceivedByPlayer(EComboState State);

	UFUNCTION()
	void StopAILogic();

	// Set Enemy color when received damage
	UFUNCTION()
	void SetDamagedColor();

	// Dead disolver function
	UFUNCTION(BlueprintCallable)
	void EnemyDissolveAfterDeath(float Timer = 2.0f);

	SATORIEnemyType GetEnemyType() { return EnemyType; }

	UFUNCTION()
	void CheckPlayerIsDeathState(bool isDeath);
protected:

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI|GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI|GAS")
	TArray<TSubclassOf<USATORI_GameplayAbility>> AICharacterAbilities;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr <UBehaviorTree> BehaviorTree;

	UPROPERTY()
	AAIController* EnemyAIController = nullptr;

	// Enemy Type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	SATORIEnemyType EnemyType;

	// Enemy Type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	SATORIEnemyImpactType EnemyImpactType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* HeadComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxRange = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageCounter = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isInFrontPlayer = false;

	// Material
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	float TimeCountDown = 3.0f;
	float LocalRate = 0.05f;
	FTimerHandle MaterialWaitHandle;

	// Check if player Death
	bool CheckPlayerDeath = false;

public: //Target System Interface related and Tag Abilities related (Nacho)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tag")
	FGameplayTag  EnemyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	bool bIsTargetable = true;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	bool IsTargetable() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	void RegisterInTargetableArray();
};
