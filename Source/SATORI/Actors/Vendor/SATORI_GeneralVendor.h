// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SATORI_InteractInterface.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_GeneralVendor.generated.h"

class UWidgetComponent;

UCLASS()
class SATORI_API ASATORI_GeneralVendor : public AActor, public ISATORI_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_GeneralVendor();

	// Virtual function 
	virtual void Interact(AActor* ActorInteracting) override;
	virtual void StartCanInteract(AActor* ActorInteracting) override;
	virtual void StopCanInteract(AActor* ActorInteracting) override;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GeneralVendorMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* VendorOpen = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* VendorClose = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void CheckPanelIsOpenedStatus(bool Value);

	// Variables
	bool isPanelOpened = false;
};
