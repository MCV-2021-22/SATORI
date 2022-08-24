// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SATORI_DummyActor.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SATORI_API ASATORI_DummyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_DummyActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UFUNCTION(BlueprintCallable)
	void PlayAnimationsWithAttack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
