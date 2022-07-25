//

#include "Actors/AbilitiesActors/SATORI_BlackHoleActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
//Debug
#include "DrawDebugHelpers.h"

ASATORI_BlackHoleActor::ASATORI_BlackHoleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshInner"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapCollisionSphere);

	//NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	//NiagaraComponent->SetupAttachment(RootComponent);

	//RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	//RadialForceComponent->SetupAttachment(CollisionSphereComponent);
	//RadialForceComponent->Radius = 512.0f;
	//RadialForceComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
	//RadialForceComponent->ForceStrength = -1000000.0f;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->ProjectileGravityScale = -0.01f;

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_BlackHoleActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// BlackHole possible collisions : 
	// Enemies
	// Objects?

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character) 
	{
		ProjectileMovementComponent->StopMovementImmediately();
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(TrappedTag))
	{
		 
	}
}

void ASATORI_BlackHoleActor::DestroyMyself()
{
	//SetActorTickEnabled(false);
	//StaticMeshComponent->SetVisibility(false);
	//NiagaraComponent->Deactivate();

	//NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystemExplode, GetRootComponent()->GetComponentLocation());
	//NiagaraComponent->Activate();

	//FScriptDelegate Delegate;
	//Delegate.BindUFunction(this, TEXT("OnNiagaraFinished"));
	//NiagaraComponent->OnSystemFinished.AddUnique(Delegate);

	Destroy();
}

void ASATORI_BlackHoleActor::OnNiagaraFinished()
{
	Destroy();
}

void ASATORI_BlackHoleActor::StopGrowing()
{
	bGrowing = false;
	ProjectileMovementComponent->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TimerHandleAttraction, this, &ASATORI_BlackHoleActor::StopAttraction, TimeToStopAttraction, false);
}

void ASATORI_BlackHoleActor::StopAttraction()
{
	bShouldAttract = false;
	//RadialForceComponent->Radius = 64.0f;
	//RadialForceComponent->ForceStrength = 500000.0f;

	CollisionSphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	for (AActor* Actor : ArrayActorsTrapped) {

		if (IsValid(Actor))
		{
			//Damage Explosion Calculation
			ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
			float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, DamageExplosion, Actor, DamageGameplayEffect);
			Character->sendDamage(DamageDone);

			//Return to normal size
			//Actor->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

			//Sometimes they get stuck together, trying this to fix it
			//FVector PrecautionMeasure = FMath::VRand();
			//PrecautionMeasure.Z = 0.0f;
			//Actor->AddActorLocalOffset(PrecautionMeasure * 400);

			Character->RemoveGameplayTag(TrappedTag);
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_BlackHoleActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_BlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	AngleAxis = 0;

	//SetActorScale3D(ScaleGrowing);
	GetWorldTimerManager().SetTimer(TimerHandleGrowing, this, &ASATORI_BlackHoleActor::StopGrowing, TimeToStopGrowing, false);
}

void ASATORI_BlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bGrowing)
	//{
	//	//Starts small, grows along thew way
	//	ScaleGrowing = ScaleGrowing + (Increment * DeltaTime);
	//	SetActorScale3D(ScaleGrowing);

	//	//Stay grounded calculation
		FVector ActorPosition = GetActorLocation();
		FVector End = ActorPosition;
		End.Z -= TraceDistanceToFloor;
		bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);
		if (bDrawDebug)
		{
			DrawDebugLine(GetWorld(), ActorPosition, End, bHitAnything ? FColor::Green : FColor::Red, false, 1.0f);
		}
		if (bHitAnything) {
			if (OutHit.Distance < MinHeight) {
				ActorPosition.Z += HeightChange * DeltaTime;
				SetActorLocation(ActorPosition);
			}
		}
	//}

	if (bShouldAttract)
	{
		//Adding more force to the black hole
		for (AActor* Actor : ArrayActorsTrapped) {

			if (IsValid(Actor))
			{

				FVector CenterPosition = GetActorLocation();
				AngleAxis += SpeedRotation * DeltaTime;
				if (AngleAxis >= 360) AngleAxis = 0;
				

				FVector RotateValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);

				CenterPosition.X += RotateValue.X;
				CenterPosition.Y += RotateValue.Y;
				CenterPosition.Z += RotateValue.Z;

				Actor->SetActorLocation(CenterPosition, false, 0, ETeleportType::None);


				//This first part sometimes causes problems
				//FVector CenterPosition = GetActorLocation();
				//FVector VectorToCenter = Actor->GetActorLocation() - CenterPosition;

				//FVector Rotation = VectorToCenter.RotateAngleAxis(DeltaTime * RotationSpeed, CenterPosition);
				//Rotation.Normalize();

				//Actor->AddActorWorldOffset(Rotation);

				//This part is necesary for correct behavior
/*				FVector ActorPosition = Actor->GetActorLocation();
				VectorToCenter = ActorPosition - GetActorLocation();

				Actor->SetActorLocation(ActorPosition - (VectorToCenter * DeltaTime * 5));*/ // 5 Little bit or risk of weird behavior // 10 Not weird behavior, but less random attraction...

				//Scale down enemies
				//FVector Scale = Actor->GetActorScale3D();
				//FVector NewScale = Scale - (Scale * DeltaTime * Decrement);

				//Actor->SetActorScale3D(NewScale);

				//Damage Calculation
				ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
				float DamageDone = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Actor, Damage, Actor, DamageGameplayEffect);
				Character->sendDamage(DamageDone);
			}
		}
	}
	else
	{
		//CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ScaleGrowing = ScaleGrowing + (Increment * ExplosionSize * DeltaTime);
		//SetActorScale3D(ScaleGrowing);
	}
}