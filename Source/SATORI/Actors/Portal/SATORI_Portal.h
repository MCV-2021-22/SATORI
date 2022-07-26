// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTags.h"
#include "SATORICharacter.h"
#include "Interfaces/SATORI_InteractInterface.h"
#include "SATORI_Portal.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UTextRenderComponent;
class USATORI_MainUI;
class USATORI_DoorInteractUI;
class USATORI_PortalPassiveDataAsset;
class UBillboardComponent;
class UWidgetComponent;

USTRUCT(BlueprintType)
struct FSATORI_PortalAbilitiesDatasReward
{
	GENERATED_BODY()

	TSubclassOf<USATORI_GameplayAbility> CurrentAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* AbilitiyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isUpgrated;
};

USTRUCT(BlueprintType)
struct FSATORI_DoorPassiveReward
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> PassiveEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* PassiveIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
};

UCLASS()
class SATORI_API ASATORI_Portal : public AActor, public ISATORI_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_Portal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	// Virtual function 
	virtual void Interact(AActor* ActorInteracting) override;
	virtual void StartCanInteract(AActor* ActorInteracting) override;
	virtual void StopCanInteract(AActor* ActorInteracting) override;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* TextRenderComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBillboardComponent* PortalIconTexture = nullptr;

	// Effect apply to player 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayEffect")
	FSATORI_DoorPassiveReward PortalEffectsToApply;

	// Ability adding to player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayAbility")
	FSATORI_PortalAbilitiesDatasReward PortalAbilityToApply;

	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USATORI_MainUI> SATORIMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	USATORI_DoorInteractUI* DoorInteractUI;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PassiveDataAsset")
	USATORI_PortalPassiveDataAsset* PassiveDataAsset;*/

	void SetCurrentGameplayEffectData(FSATORI_DoorPassiveReward CurrentEffecData);
public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplyEffectToPlayer(AActor* PlayerCharacter);

	TSubclassOf<UGameplayEffect> GetCurrentGameplayEffect();

private:
	TSubclassOf<UGameplayEffect> CurrentGameplayEffect;

public:

	void ActivatePortal();

	void ChangeLevel(ASATORICharacter* Character);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	TArray < FString > LevelNames;

private:

	bool Active = false;

};
