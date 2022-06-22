//

#include "Actors/AbilitiesActors/SATORI_PushActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_PushActor::ASATORI_PushActor()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PushActor::OnOverlapSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_PushActor::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	//Missile possible collisions : 
	// Enemies
	// Walls
	// Enemy Proyectiles?
	// Objects?

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	//Walls //EnemyProyectiles? //Objects?
	if (!Character) 
	{
		DestroyMyself();
		return;
	}

	//Enemies
	if(Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(PushedTag))
	{	
		ArrayPushed.AddUnique(OtherActor);
		Character->AddGameplayTag(PushedTag);
	}
}

void ASATORI_PushActor::DestroyMyself()
{	

	CollisionSphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	for (AActor* Actor : ArrayPushed) {
		if (IsValid(Actor))
		{
			ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
			Character->RemoveGameplayTag(PushedTag);
		}
	}
	Destroy();
}

void ASATORI_PushActor::BeginPlay() 
{

	Super::BeginPlay();

	if(!EnemyTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_PushActor: Tag is not valid ... "), *GetName());
	}

	//Set max time before auto destruc if not collides
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_PushActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_PushActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();
	FVector ActorForward = GetActorForwardVector();
	SetActorLocation(ActorPosition + ActorForward * Speed * DeltaTime);

	//Pushing
	for (AActor* Actor : ArrayPushed) {
		if (IsValid(Actor))
		{
			Actor->SetActorLocation(Actor->GetActorLocation() + ActorForward * PushForce * DeltaTime);
			//Damage Calculation
			ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
			float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
			Character->sendDamage(DamageDone);
		}
	}

	//Stay grounded calculation
	ActorPosition = GetActorLocation();
	FVector End = ActorPosition;
	End.Z -= TraceDistanceToFloor;
	bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), ActorPosition, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
	}
	if (bHitAnything) {
		if (OutHit.Distance > MaxHeight) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
		if (OutHit.Distance < MinHeight) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
	}
}

