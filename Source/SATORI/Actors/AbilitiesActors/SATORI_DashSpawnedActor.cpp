//

#include "SATORI_DashSpawnedActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "SATORICharacter.h"
#include "SATORIGameMode.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

ASATORI_DashSpawnedActor::ASATORI_DashSpawnedActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = CollisionBoxComponent;
	CollisionBoxComponent->SetCollisionProfileName(FName(TEXT("IgnoreAllOverlapOnlyAI")));
	CollisionBoxComponent->SetGenerateOverlapEvents(true);
	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashSpawnedActor::OnOverlapCollisionBox);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	Decal->SetupAttachment(RootComponent);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreAllOverlapOnlyPlayer")));
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashSpawnedActor::OnOverlapCollisionSphere);

	//Debug
	CollisionBoxComponent->bHiddenInGame = true;
	CollisionSphereComponent->bHiddenInGame = true;
}

void ASATORI_DashSpawnedActor::OnOverlapCollisionBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If Melee overlaps ends dash
	if(IsValid(OwnerSpawned) && OwnerSpawned == OtherActor)
	{
		ASATORI_Spawned* Spawned = Cast<ASATORI_Spawned>(OtherActor);
		if (Spawned)
		{
			Spawned->AddGameplayTag(DashStopTag);
		}
	}
}

void ASATORI_DashSpawnedActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Damage player once
	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(OtherActor);

	if (!Character)
	{
		return;
	}
	if (Character && !bDamagePlayer)
	{
		bDamagePlayer = true;
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Character, Damage, Character, DamageGameplayEffect);
		USATORI_BlueprintLibrary::ApplyGameplayEffect(Character, HeavyHitGameplayEffect);
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), EventExplosion, true);
		if (SpawnedCharacter)
		{
			FVector SpawnLocation = SpawnedCharacter->GetActorLocation();
			if (SpawnedCharacter->NS_ParticleEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnedCharacter->NS_ParticleEffect, SpawnLocation);
			}
		}
		Destroy();
		if (SpawnedCharacter && SpawnedCharacter->MySpawn) 
			SpawnedCharacter->MySpawn->AddNumEnemies(-1);
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(OwnerSpawned);
		OwnerSpawned->Destroy();
	}
}

void ASATORI_DashSpawnedActor::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerSpawned)
	{
		SpawnedCharacter = Cast<ASATORI_Spawned>(OwnerSpawned);
	}
}

void ASATORI_DashSpawnedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//When Melee is close enough starts attack parte of animation
	if (!bEndDash && FVector::Dist(OwnerSpawned->GetActorLocation(), GetActorLocation()) < 150.0f)
	{
		bEndDash = true;

		if (USkeletalMeshComponent* Mesh = SpawnedCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				AnimInstance->Montage_JumpToSection(FName("explotion"), AnimInstance->GetCurrentActiveMontage());
			}
		}
	}

	//Enables sphere overlaps events for damaging player
	if (bEndDash && !CollisionSphereComponent->IsCollisionEnabled())
	{
		if (SpawnedCharacter->HasMatchingGameplayTag(DashDamageTag))
		{
			CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
}