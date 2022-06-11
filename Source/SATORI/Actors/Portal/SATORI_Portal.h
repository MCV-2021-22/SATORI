// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTags.h"
#include "SATORI_Portal.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UTextRenderComponent;
class USATORI_MainUI;
class USATORI_DoorInteractUI;
class USATORI_PortalPassiveDataAsset;

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
	FText Desciption;
};

UCLASS()
class SATORI_API ASATORI_Portal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_Portal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* TextRenderComponent = nullptr;

	// Effect apply to player 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayEffect")
	TArray<FSATORI_DoorPassiveReward> PortalEffectsToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PlayerTag;

	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USATORI_MainUI> SATORIMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	USATORI_DoorInteractUI* DoorInteractUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PassiveDataAsset")
	USATORI_PortalPassiveDataAsset* PassiveDataAsset;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplyEffectToPlayer(AActor* PlayerCharacter);

private:
	TSubclassOf<UGameplayEffect> SelectRandomEffect();
};
