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
	if(Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(LaunchTag))
	{	
		StopAction(Character);
		RotateEnemy(OtherActor);
		LaunchEnemy(OtherActor, Character);
		DamageEnemy(OtherActor, Character);
	}
}

//Stops ability and  animation if active
void ASATORI_Push360Actor::StopAction(ASATORI_AICharacter* Character)
{
	Character->RemoveGameplayTag(AbilityTag);
	UAnimMontage* AnimMontage = Character->GetCurrentMontage();
	if (IsValid(AnimMontage))
	{
		Character->StopAnimMontage(AnimMontage);
	}
}

void ASATORI_Push360Actor::LaunchEnemy(AActor* Actor, ASATORI_AICharacter* Character)
{
	FVector Location = Actor->GetActorLocation();
	Location.Z += 100;
	Actor->SetActorLocation(Location);

	FVector LaunchDirection = Actor->GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	LaunchDirection.Z = ZLaunching;
	LaunchDirection.Normalize();
	Character->LaunchCharacter(LaunchDirection * LaunchForce, true, true);

	Character->AddGameplayTag(LaunchTag);
}

void ASATORI_Push360Actor::RotateEnemy(AActor* Actor)
{
	FVector RotationDirection = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - Actor->GetActorLocation();
	RotationDirection.Normalize();
	FRotator Rotator = RotationDirection.Rotation();
	Rotator.Pitch = 0;
	Rotator.Roll = 0;
	Actor->SetActorRotation(Rotator);
}

void ASATORI_Push360Actor::DestroyMyself()
{
	Destroy();
}

void ASATORI_Push360Actor::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandleDestroy;
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_Push360Actor::DestroyMyself, TimeToFinish, false);
}

void ASATORI_Push360Actor::DamageEnemy(AActor* Actor, ASATORI_AICharacter* Character)
{
	USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	Character->CheckDamage();
}
