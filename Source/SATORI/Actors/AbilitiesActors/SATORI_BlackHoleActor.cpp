//

#include "Actors/AbilitiesActors/SATORI_BlackHoleActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_BlackHoleActor::ASATORI_BlackHoleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapCollisionSphere);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_BlackHoleActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BlackHole possible collisions : 
	// Enemies
	// Objects?

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(OtherActor);

	if (!Character) 
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		Character->AddGameplayTag(TrappedTag);
		ArrayTrapped.AddUnique(Character);
	}
}

void ASATORI_BlackHoleActor::DestroyMyself()
{
	for (ASATORI_CharacterBase* Character : ArrayTrapped) {
		Character->RemoveGameplayTag(TrappedTag);
	}
	Destroy();
}

void ASATORI_BlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_BlackHoleActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_BlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}