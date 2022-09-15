// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Portal/SATORI_WinPortalActor.h"
#include "Components/WidgetComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerState.h"
#include "UI/SATORI_MainUI.h"
#include "Character/SATORI_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SATORIGameMode.h"
#include "Components/SphereComponent.h"

// Sets default values
ASATORI_WinPortalActor::ASATORI_WinPortalActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// Hide interaction
	WidgetComponent->SetVisibility(false, true);
}

// Called when the game starts or when spawned
void ASATORI_WinPortalActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->AddWinPortalActor(this);

	SetCurrentMeshInteractability(false);
}

void ASATORI_WinPortalActor::Interact(AActor* ActorInteracting)
{
	if (isActive)
	{
		WinWidgetgWidget = CreateWidget<UUserWidget>(GetGameInstance(), WidgetTemplate);
		if (WinWidgetgWidget)
		{			
			WinWidgetgWidget->AddToViewport();
			ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (SatoriCharacter)
			{
				ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
				if (PlayerController)
				{
					PlayerController->bShowMouseCursor = true;
					PlayerController->bEnableClickEvents = true;
					PlayerController->bEnableMouseOverEvents = true;
					SatoriCharacter->DisableInput(PlayerController);
				}
			}
		}
	}
}

void ASATORI_WinPortalActor::StartCanInteract(AActor* ActorInteracting)
{
	WidgetComponent->SetVisibility(true, true);
}

void ASATORI_WinPortalActor::StopCanInteract(AActor* ActorInteracting)
{
	WidgetComponent->SetVisibility(false, true);
}

void ASATORI_WinPortalActor::ActivatePortal()
{
	isActive = true;
}

void ASATORI_WinPortalActor::SetCurrentMeshInteractability(bool CanInteract)
{
	if (CanInteract)
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
		//UE_LOG(LogTemp, Warning, TEXT("TRUE!!!!!!!!!!!!!!!!!"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("FALSE!!!!!!!!!!!!!!!!!"));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}
}