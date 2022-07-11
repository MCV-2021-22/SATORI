// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_PortalStart.h"
#include "SATORI/SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SATORI/Character/SATORI_PlayerState.h"
#include "UI/WorldActor/SATORI_DoorInteractUI.h"
#include "UI/SATORI_MainUI.h"
#include "Data/SATORI_PortalPassiveDataAsset.h"
#include "Components/BillboardComponent.h"
#include <algorithm>
#include "SATORIGameMode.h"
#include "GameplayFramework/SATORI_GameInstance.h"

// Sets default values
ASATORI_PortalStart::ASATORI_PortalStart()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);

	PortalIconTexture = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	PortalIconTexture->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(120.0f);
	SphereComponent->SetCollisionProfileName(FName("IgnoreAllOverlapOnlyPlayer"));
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_PortalStart::OnComponentBeginOverlap);

}

// Called when the game starts or when spawned
void ASATORI_PortalStart::BeginPlay()
{
	Super::BeginPlay();

}

void ASATORI_PortalStart::OnConstruction(const FTransform& Transform)
{
}

void ASATORI_PortalStart::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Overlap Beg %s"), *OtherActor->GetName());

	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character)
	{
		ChangeLevel(Character);
	}
}

void ASATORI_PortalStart::ChangeLevel(ASATORICharacter* Character)
{

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstanceRef)
	{
		GameInstanceRef->Health = Character->GetMaxHealth();
		GameInstanceRef->MaxHealth = Character->GetMaxHealth();
		GameInstanceRef->Mana = Character->GetMaxMana();
		GameInstanceRef->MaxMana = Character->GetMaxMana();
		GameInstanceRef->Defense = Character->GetDefense();
		GameInstanceRef->Attack = Character->GetAttack();
		GameInstanceRef->MoveSpeed = Character->GetMoveSpeed();
		GameInstanceRef->Gold = Character->GetGold();
	}

	if (LevelNames.Num() != 0 )
	{
		int NumLevels = LevelNames.Num() - 1;
		int  Level = FMath::RandRange(0, NumLevels);
		UGameplayStatics::OpenLevel(GetWorld(), FName(LevelNames[Level]));
	}
}