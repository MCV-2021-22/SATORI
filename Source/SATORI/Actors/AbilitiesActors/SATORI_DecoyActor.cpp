//

#include "Actors/AbilitiesActors/SATORI_DecoyActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_DecoyActor::ASATORI_DecoyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	//If collides will lure enemies
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DecoyActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

//Collision for luring
void ASATORI_DecoyActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BlackHole possible collisions : 
	// Enemies

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);
	
	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		Character->AddGameplayTag(LuredTag);
		ArrayLured.AddUnique(Character);
	}
}

void ASATORI_DecoyActor::DestroyMyself()
{
	for (ASATORI_AICharacter* Character : ArrayLured)
	{
		Character->RemoveGameplayTag(LuredTag);
	}
	//Add collision for damage explosion
	Destroy();
}

void ASATORI_DecoyActor::BeginPlay()
{
	Super::BeginPlay();

	//Set max time before auto destruc
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_DecoyActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_DecoyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

