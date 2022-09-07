// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SATORI_BossAlertActor.generated.h"

class ASATORICharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSATORIEnterBossCombat, bool, IsEntering);

UCLASS()
class SATORI_API ASATORI_BossAlertActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_BossAlertActor();

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* Trigger;

	UPROPERTY(BlueprintAssignable)
	FSATORIEnterBossCombat PlayerEnteringCombat;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ASATORICharacter* PlayerCharacter = nullptr;
};
