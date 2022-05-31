
#include "Actors/AbilitiesActors/SATORI_PullActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

// Sets default values
ASATORI_PullActor::ASATORI_PullActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	//If collides will grab
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PullActor::OnOverlapCollisionSphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->bIsHomingProjectile = true;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

//Collision for Grabing
void ASATORI_PullActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		Destroy();
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->sendDamage(dmg_done);
		Pulling = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		GetWorldTimerManager().ClearTimer(TimerHandleDestroy);
	}
	if (!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		DestroySelf();
	}
}

// Called when the game starts or when spawned
void ASATORI_PullActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!EnemyTag.IsValid() || !PlayerTag.IsValid() || !TargetActorWithTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_PullActor: Tag is not valid ... "), *GetName());
	}

	//Check if Player is currently targeting an enemy
	////
	//TO DO: 
	////
	//Check Nearest Actor in viewport
	//Improve code if possible
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORI_CharacterBase::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(Actor);
		if (Character->HasMatchingGameplayTag(TargetActorWithTag))
		{
			Target = Actor;
		}
		else
		{
			if (Character->HasMatchingGameplayTag(PlayerTag))
			{
				Player = Actor;
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
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_PullActor::DestroySelf, TimeToDestroy, false);

}

// Called every frame
void ASATORI_PullActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();
	//If has grabbed adn enemy
	if (Pulling)
	{
		////
		//TO DO: 
		////
		//Improve overall behavior
		ProjectileMovementComponent->HomingTargetComponent = Player->GetRootComponent();

		ActorPosition = GetActorLocation();
		Pulling->SetWorldLocation(ActorPosition);

		//If has reached the player
		if (FVector::Dist(Player->GetActorLocation() , ActorPosition) < 250) {
			DestroySelf();
		}
	}
}

void ASATORI_PullActor::DestroySelf()
{
	Destroy();
}
