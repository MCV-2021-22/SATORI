// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SATORI_InteractInterface.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_MaskVendor.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;

UCLASS()
class SATORI_API ASATORI_MaskVendor : public AActor, public ISATORI_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_MaskVendor();

	// Virtual function 
	virtual void Interact(AActor* ActorInteracting) override;
	virtual void StartCanInteract(AActor* ActorInteracting) override;
	virtual void StopCanInteract(AActor* ActorInteracting) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MaskVendorMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* MaskOpen = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* MaskClose = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CheckPanelIsOpenedStatus(bool Value);

	// Variables
	bool isPanelOpened = false;
};
