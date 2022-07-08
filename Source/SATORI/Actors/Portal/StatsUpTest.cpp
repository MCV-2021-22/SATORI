// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Portal/StatsUpTest.h"
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
#include "Components/Player/SATORI_StatsComponent.h"
#include "SATORIGameMode.h"
#include "GameplayFramework/SATORI_GameInstance.h"

// Sets default values
AStatsUpTest::AStatsUpTest()
{

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(120.0f);
	SphereComponent->SetCollisionProfileName(FName("IgnoreAllOverlapOnlyPlayer"));

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AStatsUpTest::OnComponentBeginOverlap);

}

// Called when the game starts or when spawned
void AStatsUpTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStatsUpTest::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
		ApplyEffectToPlayer(Character);
	}
}

void AStatsUpTest::ApplyEffectToPlayer(AActor* PlayerActor)
{
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(PlayerActor);
	if (PlayerCharacter)
	{
		UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect,
				PlayerCharacter->GetCharacterLevel(), EffectContext);

			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
					AbilitySystemComponent);
			}
		}
	}
}