// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Preloader/SATORI_BossAlertActor.h"
#include "Components/CapsuleComponent.h"
#include "SATORICharacter.h"
#include "Character/SATORI_PlayerState.h"
#include "Character/SATORI_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/SATORI_MainUI.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

// Sets default values
ASATORI_BossAlertActor::ASATORI_BossAlertActor()
{
	RootComponent = Trigger;

	Trigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	Trigger->InitCapsuleSize(55.f, 96.0f);;
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BossAlertActor::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASATORI_BossAlertActor::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ASATORI_BossAlertActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASATORI_BossAlertActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		PlayerCharacter = Cast<ASATORICharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->GetComboSystemComponent()->isInBossFight = true;
			ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(PlayerCharacter->GetController());
			if (PlayerController)
			{
				USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
				if (MainUI)
				{
					MainUI->SetBossUIVisibility(true);
				}
			}
		}
		this->Destroy();
	}
}

void ASATORI_BossAlertActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (OtherActor && (OtherActor != this) && OtherComp)
	{
		PlayerCharacter = Cast<ASATORICharacter>(OtherActor);
		if (PlayerCharacter)
		{
			ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(PlayerCharacter->GetController());
			if (PlayerController)
			{
				USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
				if (MainUI)
				{
					MainUI->SetBossUIVisibility(false);
				}
			}
		}
	}*/
}

