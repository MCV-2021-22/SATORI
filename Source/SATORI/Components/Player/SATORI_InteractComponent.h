// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SATORI_InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_InteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USATORI_InteractComponent();

	UPROPERTY(EditDefaultsOnly)
	float InteractionDistance = 230.0f;

	UPROPERTY(BlueprintReadOnly, Transient)
	TWeakObjectPtr<AActor> CurrentActor = nullptr;

	// The collision channel used when query the world for object to interact with
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> CollisionChannel;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryToInteract();

};
