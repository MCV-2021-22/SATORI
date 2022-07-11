// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "SATORICharacter.h"
#include "GameFramework/Actor.h"
#include "StatsUpTest.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;
class UTextRenderComponent;
class USATORI_MainUI;
class USATORI_DoorInteractUI;
class USATORI_PortalPassiveDataAsset;
class UBillboardComponent;

UCLASS()
class SATORI_API AStatsUpTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatsUpTest();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> Effect;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplyEffectToPlayer(AActor* PlayerCharacter);

protected:

	virtual void BeginPlay() override;


};
