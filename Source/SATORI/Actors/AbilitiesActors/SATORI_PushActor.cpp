//

#include "Actors/AbilitiesActors/SATORI_PushActor.h"
#include "Components/BoxComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
//Debug
#include "DrawDebugHelpers.h"
#include <AI/Character/Spawned/SATORI_Spawned.h>

ASATORI_PushActor::ASATORI_PushActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBoxComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionBoxComponent->SetupAttachment(RootComponent);
	CollisionBoxComponent->SetGenerateOverlapEvents(true);
	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PushActor::OnOverlapCollisionBox);

	//Debug
	CollisionBoxComponent->bHiddenInGame = false;
}

void ASATORI_PushActor::OnOverlapCollisionBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Possible collisions : 

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	//Walls other objects
	if (!Character) 
	{
		DestroyMyself();
		return;
	}

	ASATORI_Spawner* Spawner = Cast<ASATORI_Spawner>(Character);
	if (Spawner) return;

	//Enemies
	if(Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(PushedTag) && !Pushing && !Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")) && !Character->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Boss.InmunityHabs")))
	{	
		Pushing = true;
		StopAction(Character);
		PushedActor = OtherActor;
		PushedCharacter = Character;
		Character->AddGameplayTag(PushedTag);
	}
}

//Stops ability and animation if active
void ASATORI_PushActor::StopAction(ASATORI_AICharacter* Character)
{
	//Edge cases
	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(SpecialTag);
	GameplayTagContainer.AddTag(StunnedTag);
	USATORI_BlueprintLibrary::RemoveGameplayEffect(Character, GameplayTagContainer);

	Character->RemoveGameplayTag(AbilityTag);
	UAnimMontage* AnimMontage = Character->GetCurrentMontage();
	if (IsValid(AnimMontage))
	{
		Character->StopAnimMontage(AnimMontage);
	}
	
}

void ASATORI_PushActor::DestroyMyself()
{
	if (IsValid(PushedActor))
	{
		RotateEnemy(PushedActor);
		LaunchEnemy(PushedActor, PushedCharacter);
	}

	Destroy();
}

void ASATORI_PushActor::RotateEnemy(AActor* Actor)
{
	FVector RotationDirection = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - Actor->GetActorLocation();
	RotationDirection.Normalize();
	FRotator Rotator = RotationDirection.Rotation();
	Rotator.Pitch = 0;
	Rotator.Roll = 0;
	Actor->SetActorRotation(Rotator);
}

void ASATORI_PushActor::LaunchEnemy(AActor* Actor, ASATORI_AICharacter* Character)
{
	Character->RemoveGameplayTag(PushedTag);

	FVector LaunchDirection = Actor->GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	LaunchDirection.Z = ZLaunching;
	LaunchDirection.Normalize();

	Character->LaunchCharacter(LaunchDirection * LaunchForce, true, true);

	Character->AddGameplayTag(LaunchTag);
}

void ASATORI_PushActor::BeginPlay() 
{
	Super::BeginPlay();

	if (!EnemyTag.IsValid() || !PushedTag.IsValid() || !LaunchTag.IsValid() || !AbilityTag.IsValid() || !StunnedTag.IsValid() || !SpecialTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_PushActor: Tag not valid ... "), *GetName());
		Destroy();
	}

	FTimerHandle TimerHandleDestroy;
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_PushActor::DestroyMyself, TimeToFinish, false);

	FVector SpawnLocation = CollisionBoxComponent->GetComponentLocation();

	if (Push_Particle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Push_Particle, SpawnLocation);
	}
}

void ASATORI_PushActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();
	FVector ActorForward = GetActorForwardVector();
	SetActorLocation(ActorPosition + ActorForward * Speed * DeltaTime);

	StayGrounded(DeltaTime);

	
	
	if (IsValid(PushedActor))
	{

		//Move enemy
		HeightCorrection += DeltaTime;
		ActorPosition.Z += HeightCorrection;
		PushedActor->SetActorLocation(ActorPosition, false, 0, ETeleportType::TeleportPhysics);

		DamageEnemy(PushedActor, PushedCharacter);
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
void ASATORI_PushActor::DamageEnemy(AActor* Actor, ASATORI_AICharacter* Character)
{
	USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
	Character->CheckDamage(Damage);
}
