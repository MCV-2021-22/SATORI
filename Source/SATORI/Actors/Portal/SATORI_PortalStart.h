// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTags.h"
#include "SATORICharacter.h"
#include "SATORI_PortalStart.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UTextRenderComponent;
class USATORI_MainUI;
class USATORI_DoorInteractUI;
class USATORI_PortalPassiveDataAsset;
class UBillboardComponent;

UCLASS()
class SATORI_API ASATORI_PortalStart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_PortalStart();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* TextRenderComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBillboardComponent* PortalIconTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PlayerTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	TArray < FString > LevelNames;

	// Widgets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USATORI_MainUI> SATORIMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	USATORI_DoorInteractUI* DoorInteractUI;

public:

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ChangeLevel(ASATORICharacter* Character);

};
