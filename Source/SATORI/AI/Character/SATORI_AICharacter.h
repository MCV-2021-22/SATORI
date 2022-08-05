// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/SATORI_TargetSystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Character/SATORI_CharacterBase.h"
#include "GameplayTags.h"
#include "GAS\SATORI_GameplayAbility.h"
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

UENUM(BlueprintType)
enum class SATORIEnemyType : uint8
{
	None UMETA(DisplayName = "None"),
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Boss UMETA(DisplayName = "Boss"),
};


UCLASS()
class SATORI_API ASATORI_AICharacter : public ASATORI_CharacterBase, public ISATORI_TargetSystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASATORI_AICharacter();

	// Ray Cast
	UFUNCTION(BlueprintCallable)
	bool CheckPlayerWithRayCast();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// Initialize AI Attributes from GE
	void InitializeAttributes();

	// Adding Abilities to AI
	virtual void AddAICharacterAbilities();

	void GrantAbilityToPlayer(FGameplayAbilitySpec Ability);

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:
	UPROPERTY()
	USATORI_AttributeSet* AttributeSet;

	// Character Default Abilities Asset (Contain List of Player Abilities)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|GAS")
	USATORI_AbilityDataAsset* DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness)
	UPawnSensingComponent* PawnSensor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCapsuleComponent* AttackingCollision2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	USATORI_EnemyStatComponent* EnemyStatComponent;

	bool GetIsInFront() const { return isInFrontPlayer; }

	UFUNCTION(BlueprintCallable)
	void sendDamage(float dmg);

	float getDistAttack();

	float getMaxRangeDist();

	UFUNCTION(BlueprintCallable)
	void HealthBarProjection(UWidgetComponent* HealthBar, float ViewDistance, float RangeA, float RangeB);

	// Default attributes for a character for initializing
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<USATORI_EnemyHealthBar> HealthBarUI;
	
protected:
	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI|GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI|GAS")
	TArray<TSubclassOf<USATORI_GameplayAbility>> AICharacterAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponent2;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr <UBehaviorTree> bte;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBehaviorTree* btree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float dist_attack = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float max_range_dist = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isInFrontPlayer = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	SATORIEnemyType EnemyType;

	float dmg_burst = 0.f;

	float time_burst = 5.f;

	bool Bursting = false;

	float dmg_stun = 0.f;

	float time_stun = 5.f;

	bool Stunning = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* HeadComponent;

	
public: //Target System Interface related and Tag Abilities related (Nacho)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tag")
	FGameplayTag  EnemyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	bool bIsTargetable = true;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	bool IsTargetable() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Target System")
	void RegisterInTargetableArray();

public: //Character Death

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Death")
	UAnimMontage* DeathMontage;

	virtual void CharacterDeath() override;

	virtual void RemoveCharacterAbilities() override;

	void DestroyMyself();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tag")
	FGameplayTagContainer   BlockTags;

	UFUNCTION(BlueprintCallable)
	bool GetBurstingState();

	UFUNCTION(BlueprintCallable)
	void SetBurstingFalse();

};
