// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SATORI_InteractInterface.h"
#include "SATORI_WinPortalActor.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class SATORI_API ASATORI_WinPortalActor : public AActor, public ISATORI_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_WinPortalActor();

	// Virtual function 
	virtual void Interact(AActor* ActorInteracting) override;
	virtual void StartCanInteract(AActor* ActorInteracting) override;
	virtual void StopCanInteract(AActor* ActorInteracting) override;
	
	void ActivatePortal();

	void SetCurrentMeshInteractability(bool CanInteract);

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* WidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading Screen")
	TSubclassOf<UUserWidget> WidgetTemplate;

	UPROPERTY()
	UUserWidget* WinWidgetgWidget = nullptr;

	bool isActive = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
