//

#include "Actors/AbilitiesActors/SATORI_BlackHoleActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_BlackHoleActor::ASATORI_BlackHoleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapCollisionSphere);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->SetupAttachment(CollisionSphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

void ASATORI_BlackHoleActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character) 
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->GameplayTags.AddTag(TagToAddWhenTrapped);
		ArrayTrapped.AddUnique(Character);
	}
	if (!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ZeroVector);
	}
}

void ASATORI_BlackHoleActor::DestroyMyself()
{
	for (ASATORI_AICharacter* Character : ArrayTrapped) {
		Character->GameplayTags.RemoveTag(TagToAddWhenTrapped);
	}
	Destroy();
}

void ASATORI_BlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyTag.IsValid() || !PlayerTag.IsValid() || !TargetActorWithTag.IsValid() || !TagToAddWhenTrapped.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_BlackHoleActor: Tag is not valid ... "), *GetName());
	}

	//Check if Player is currently targeting an enemy
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorWithTag.GetTagName(), Actors);
	if (Actors.Num() != 0)
	{
		Target = Actors.Pop();
	}

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_BlackHoleActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_BlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


