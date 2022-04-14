// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_Portal.h"
#include "SATORI/SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SATORI/Character/SATORI_PlayerState.h"
#include <algorithm>
// Sets default values
ASATORI_Portal::ASATORI_Portal()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(120.0f);
	SphereComponent->SetCollisionProfileName(FName("Trigger"));

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Portal::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ASATORI_Portal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASATORI_Portal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Overlap Beg %s"), *OtherActor->GetName());

	// Need change to this
	/*if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(OtherActor)) {
		if (TagInterface->HasMatchingGameplayTag(PlayerTag)) {

		}
	}*/
	if(OtherActor)
		ApplyEffectToPlayer(OtherActor);

}

void ASATORI_Portal::ApplyEffectToPlayer(AActor* PlayerActor)
{
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(PlayerActor);
	if (PlayerCharacter)
	{
		UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
		if (AbilitySystemComponent && PortalEffectsToApply.Num() > 0)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			TSubclassOf<UGameplayEffect> GameplayEffect = SelectRandomEffect();

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect,
				PlayerCharacter->GetCharacterLevel(), EffectContext);

			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
					AbilitySystemComponent);
			}
		}
	}
}

TSubclassOf<UGameplayEffect> ASATORI_Portal::SelectRandomEffect()
{
	// Change this to no repeatable number
	const int EffectSize = PortalEffectsToApply.Num() - 1;
	int number = FMath::RandRange(0, EffectSize);
	return PortalEffectsToApply[number];
}