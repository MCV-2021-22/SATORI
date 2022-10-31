// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Coin/SATORI_CoinRewardActor.h"

#include "Components/SphereComponent.h"
#include "SATORI/SATORICharacter.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ASATORI_CoinRewardActor::ASATORI_CoinRewardActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(120.0f);
	SphereComponent->SetCollisionProfileName(FName("IgnoreAllOverlapOnlyPlayer"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_CoinRewardActor::OnComponentBeginOverlap);

	Speed = 1.0f;
	RotationRate = FRotator(0.0f, 180.0f, 0.0f);
}

// Called when the game starts or when spawned
void ASATORI_CoinRewardActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASATORI_CoinRewardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));

	this->AddActorLocalRotation(RotationRate * DeltaTime * Speed);
}

void ASATORI_CoinRewardActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("On Overlap Beg %s"), *OtherActor->GetName());
	if (OtherActor)
	{
		ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(OtherActor);
		if (PlayerCharacter)
		{
			UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
			if (AbilitySystemComponent)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(CoinRewardEffect,
					PlayerCharacter->GetCharacterLevel(), EffectContext);

				if (NewHandle.IsValid())
				{
					ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
						AbilitySystemComponent);

					UFMODBlueprintStatics::PlayEvent2D(GetWorld(), EventTakeCoin, true);

					OnCoinDestroy();
				}
			}
		}
	}
}

void ASATORI_CoinRewardActor::OnCoinDestroy()
{
	FVector SpawnLocation = StaticMeshComponent->GetComponentLocation();

	if (NS_ParticleEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_ParticleEffect, SpawnLocation);
	}

	Destroy();
}