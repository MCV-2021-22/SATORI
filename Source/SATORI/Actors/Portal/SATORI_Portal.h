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

UCLASS()
class SATORI_API ASATORI_Portal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_Portal();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextRenderComponent* TextRenderComponent = nullptr;

	// Effect apply to player 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> PortalEffectsToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	FGameplayTag PlayerTag;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplyEffectToPlayer(AActor* PlayerCharacter);

private:
	TSubclassOf<UGameplayEffect> SelectRandomEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
