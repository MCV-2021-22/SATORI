//

#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "SATORICharacter.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

ASATORI_DashMeleeActor::ASATORI_DashMeleeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = CollisionBoxComponent;
	CollisionBoxComponent->SetCollisionProfileName(FName(TEXT("IgnoreAllOverlapOnlyAI")));
	CollisionBoxComponent->SetGenerateOverlapEvents(true);
	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapCollisionBox);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	Decal->SetupAttachment(RootComponent);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreAllOverlapOnlyPlayer")));
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapCollisionSphere);

	//Debug
	CollisionBoxComponent->bHiddenInGame = true;
	CollisionSphereComponent->bHiddenInGame = true;
}

void ASATORI_DashMeleeActor::OnOverlapCollisionBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If Melee overlaps ends dash
	if(IsValid(OwnerMelee) && OwnerMelee == OtherActor)
	{
		ASATORI_Melee* Melee = Cast<ASATORI_Melee>(OtherActor);
		if (Melee)
		{
			Melee->AddGameplayTag(DashStopTag);
		}
	}
}

void ASATORI_DashMeleeActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Damage player once
	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);
	if (!Character)
	{
		return;
	}

	if (Character && !bDamagePlayer)
	{
		bDamagePlayer = true;
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Character, Damage, Character, DamageGameplayEffect);
		USATORI_BlueprintLibrary::ApplyGameplayEffect(Character, HeavyHitGameplayEffect);
	}
}

void ASATORI_DashMeleeActor::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerMelee)
	{
		MeleeCharacter = Cast<ASATORI_Melee>(OwnerMelee);
	}
}

void ASATORI_DashMeleeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsValid(OwnerMelee) || !IsValid(MeleeCharacter))
	{
		return;
	}

	//When Melee is close enough starts attack parte of animation
	if (!bEndDash && FVector::Dist(OwnerMelee->GetActorLocation(), GetActorLocation()) < 150.0f)
	{
		bEndDash = true;

		if (USkeletalMeshComponent* Mesh = MeleeCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				AnimInstance->Montage_JumpToSection(FName("EndDash"), AnimInstance->GetCurrentActiveMontage());
			}
		}
	}

	//Enables sphere overlaps events for damaging player
	if (bEndDash && !CollisionSphereComponent->IsCollisionEnabled())
	{
		if (MeleeCharacter->HasMatchingGameplayTag(DashDamageTag))
		{
			CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}

	if (CollisionSphereComponent->IsCollisionEnabled() && !MeleeCharacter->HasMatchingGameplayTag(DashDamageTag))
	{
		CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}