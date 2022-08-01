//

#include "Actors/AbilitiesActors/SATORI_Push360Actor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_Push360Actor::ASATORI_Push360Actor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_Push360Actor::OnOverlapCollisionSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_Push360Actor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Possible collisions : 

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	//Walls other objects
	if (!Character) 
	{
		return;
	}

	//Enemies
	if(Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(PushedTag))
	{	
		ArrayPushed.AddUnique(OtherActor);
		Character->AddGameplayTag(PushedTag);
		LaunchEnemy(OtherActor);
		DamageEnemy(OtherActor);
	}
}

void ASATORI_Push360Actor::LaunchEnemy(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	FVector Location = Actor->GetActorLocation();
	Location.Z += 100;
	Actor->SetActorLocation(Location);
	FVector LaunchDirection = Actor->GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	LaunchDirection.Z = ZLaunching;
	LaunchDirection.Normalize();
	Character->LaunchCharacter(LaunchDirection * LaunchForce, true, true);
}

void ASATORI_Push360Actor::DestroyMyself()
{
	for (AActor* Actor : ArrayPushed) {
		if (IsValid(Actor))
		{
			ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
			Character->RemoveGameplayTag(PushedTag);
		}
	}

	Destroy();
}

void ASATORI_Push360Actor::BeginPlay()
{
	Super::BeginPlay();

	if(!EnemyTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_Push360Actor: Tag is not valid ... "), *GetName());
	}

	FTimerHandle TimerHandleDestroy;
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_Push360Actor::DestroyMyself, TimeToFinish, false);
}

//Damage Calculation
void ASATORI_Push360Actor::DamageEnemy(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	Character->sendDamage(DamageDone);
}
