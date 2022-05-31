//

#include "Actors/AbilitiesActors/SATORI_PushActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "DrawDebugHelpers.h"

ASATORI_PushActor::ASATORI_PushActor()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	RootComponent = CollisionSphereComponent;
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

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character) 
	{
		return;
	}

	if(Character->HasMatchingGameplayTag(EnemyTag))
	{
		
		float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->sendDamage(dmg_done);
		ArrayPushed.AddUnique(Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()));
	}
	if(!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		Destroy();
	}
}

void ASATORI_PushActor::DestroyMyself()
{	
	Destroy();
}

void ASATORI_PushActor::BeginPlay() 
{

	Super::BeginPlay();

	if(!EnemyTag.IsValid() || !PlayerTag.IsValid())
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

	//Weird Things Happen Here
	for (UPrimitiveComponent* RootComp : ArrayPushed) {
		RootComp->SetWorldLocation(RootComp->GetComponentLocation() + ActorForward * PushForce);
	}

	//Stay grounded calculation (Not the best)
	FHitResult OutHit;
	FVector End = ActorPosition;
	End.Z -= 250;
	FCollisionQueryParams CollisionParams;
	UWorld* World = GetWorld();
	bool bHitAnything = World->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);
	//bool bHitAnything = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);
	//DrawDebugLine(World, Start, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
	if (bHitAnything) {
	//UPrimitiveComponent* Debug = Cast<UPrimitiveComponent>(OutHit.GetComponent());
	//UE_LOG(LogTemp, Display, TEXT("Hitting: %s"), *Debug->GetName());
		if (OutHit.Distance > 50) {
			ActorPosition.Z -= 10;
			SetActorLocation(ActorPosition);
		}
		if (OutHit.Distance < 30) {
			ActorPosition.Z += 10;
			SetActorLocation(ActorPosition);
		}
	}
}

