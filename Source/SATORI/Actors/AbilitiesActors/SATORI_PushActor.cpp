//

#include "Actors/AbilitiesActors/SATORI_PushActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
//Debug
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

ASATORI_PushActor::ASATORI_PushActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PushActor::OnOverlapCollisionSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_PushActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Possible collisions : 

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	//Walls other objects
	if (!Character) 
	{
		DestroyMyself();
		return;
	}

	//Enemies
	if(Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(PushedTag) && !Pushing)
	{	
		ArrayPushed.AddUnique(OtherActor);
		Character->AddGameplayTag(PushedTag);
		Pushing = true;
	}
}

void ASATORI_PushActor::DestroyMyself()
{	
	for (AActor* Actor : ArrayPushed) {
		if (IsValid(Actor))
		{
			FinalActions(Actor);
		}
	}

	Destroy();
}

void ASATORI_PushActor::FinalActions(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	Character->RemoveGameplayTag(PushedTag);

	FVector LaunchDirection = Actor->GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	LaunchDirection.Z = ZLaunching;
	LaunchDirection.Normalize();
	Character->LaunchCharacter(LaunchDirection * LaunchForce, true, true);
}

void ASATORI_PushActor::BeginPlay() 
{
	Super::BeginPlay();

	if(!EnemyTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_PushActor: Tag is not valid ... "), *GetName());
	}

	FTimerHandle TimerHandleDestroy;
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_PushActor::DestroyMyself, TimeToFinish, false);
}

void ASATORI_PushActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();
	FVector ActorForward = GetActorForwardVector();
	SetActorLocation(ActorPosition + ActorForward * Speed * DeltaTime);

	StayGrounded(DeltaTime);
	
	for (AActor* Actor : ArrayPushed) {
		if (IsValid(Actor))
		{
			//Move enemies
			HeightCorrection += DeltaTime;
			ActorPosition.Z += HeightCorrection;
			Actor->SetActorLocation(ActorPosition, false, 0, ETeleportType::TeleportPhysics);

			DamageEnemy(Actor);
		}
	}
}

//Stay grounded calculation
void ASATORI_PushActor::StayGrounded(float DeltaTime)
{
	FVector ActorPosition = GetActorLocation();
	FVector End = ActorPosition;
	End.Z -= TraceDistanceToGround;
	bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), ActorPosition, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
	}
	if (bHitAnything) {
		if (OutHit.Distance < MinDistanceToGround) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
		if (OutHit.Distance > MaxDistanceToGround) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
	}
}

//Damage Calculation
void ASATORI_PushActor::DamageEnemy(AActor* Actor)
{
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
	float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	Character->sendDamage(DamageDone);
}
