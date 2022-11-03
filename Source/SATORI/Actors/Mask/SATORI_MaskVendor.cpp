// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Mask/SATORI_MaskVendor.h"
#include "SATORI/SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SATORI_MainUI.h"
#include "Character/SATORI_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Mask/SATORI_AbilityMask.h"

// Sets default values
ASATORI_MaskVendor::ASATORI_MaskVendor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MaskVendorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MaskVendorMesh->SetupAttachment(RootComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	// Hide interaction
	WidgetComponent->SetVisibility(false, true);
	WidgetComponent->SetRelativeLocation(FVector(50.f, 0.0f, -10.f));
}

// Called when the game starts or when spawned
void ASATORI_MaskVendor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASATORI_MaskVendor::Interact(AActor* ActorInteracting)
{
	//UE_LOG(LogTemp, Display, TEXT("Interacting with Mask Vendor"));

	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!GameInstanceRef->isInteractWithMaskVendor/* && SatoriCharacter->MaskType == SATORIMaskType::NONE*/)
	{
		if (!isPanelOpened)
		{
			isPanelOpened = true;
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MaskOpen, true);
			CheckPanelIsOpenedStatus(isPanelOpened);
		}
		else
		{
			isPanelOpened = false;
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MaskOpen, true);
			CheckPanelIsOpenedStatus(true);
		}
	}
}

void ASATORI_MaskVendor::StartCanInteract(AActor* ActorInteracting)
{
	WidgetComponent->SetVisibility(true, true);
}

void ASATORI_MaskVendor::StopCanInteract(AActor* ActorInteracting)
{
	WidgetComponent->SetVisibility(false, true);

	isPanelOpened = false;
	CheckPanelIsOpenedStatus(isPanelOpened);
}

void ASATORI_MaskVendor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}
}

void ASATORI_MaskVendor::CheckPanelIsOpenedStatus(bool IsOpened)
{
	ASATORICharacter* SatoriCharacter = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ASATORI_PlayerController* PlayerController = nullptr;
	if (SatoriCharacter)
	{
		PlayerController = Cast<ASATORI_PlayerController>(SatoriCharacter->GetController());
		if (PlayerController)
		{
			USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
			if (MainUI)
			{
				MainUI->SetMaskVendorVisibility(IsOpened);
			}
		}
	}

	// Show Mouse
	if (IsOpened && PlayerController && SatoriCharacter)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		SatoriCharacter->DisableInput(PlayerController);
		//PlayerController->SetInputMode();
	}
	else
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		SatoriCharacter->EnableInput(PlayerController);
	}
}