//

#include "Actors/AbilitiesActors/SATORI_MissileActor.h"

#include "AI/Character/Spawned/SATORI_Spawned.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_MissileActor::ASATORI_MissileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	//If collides will explode
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MissileActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->bIsHomingProjectile = true;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

//Collision for exploding
void ASATORI_MissileActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		DestroyMyself();
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		
		ProjectileMovementComponent->Velocity = (FVector::ZeroVector);

		Character->sendDamage(dmg_done);

		DestroyMyself();
	}
	if (!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		ProjectileMovementComponent->Velocity = (FVector::ZeroVector);
		DestroyMyself();
	}
}

void ASATORI_MissileActor::DestroyMyself()
{
	Destroy();
}

void ASATORI_MissileActor::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyTag.IsValid() || !PlayerTag.IsValid() || !TargetActorWithTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_MissileActor: Tag is not valid ... "), *GetName());
	}

	ProjectileMovementComponent->HomingTargetComponent = nullptr;

	//Check if Player is currently targeting an enemy
	////
	//TO DO: 
	////
	//Check Nearest Actor in viewport
	//Improve code if possible
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORI_AICharacter::StaticClass(), Actors);
	for (AActor* Actor : Actors) 
	{
		ASATORI_CharacterBase* Character = Cast<ASATORI_AICharacter>(Actor);
		if (Character->HasMatchingGameplayTag(TargetActorWithTag))
		{
			Target = Actor;
		}
		else
		{
			if (!Character->HasMatchingGameplayTag(CloneTag))
			{
				const float Distance = GetDistanceTo(Actor);
				if (Distance < Range)
				{
					Range = Distance;
					TargetNear = Actor;
				}
			}
		}
	}

	if (Target)
	{
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	}
	else
	{
		if (TargetNear)
		{
			ProjectileMovementComponent->HomingTargetComponent = TargetNear->GetRootComponent();
		}
	}

	//Set max time before auto destruc if not collides
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_MissileActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_MissileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

