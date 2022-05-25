//

#include "Actors/AbilitiesActors/SATORI_BlackHoleActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "SATORI/Character/SATORI_CharacterBase.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_BlackHoleActor::ASATORI_BlackHoleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	//If collides will explode
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapCollisionSphere);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_BlackHoleActor::OnOverlapSphereOnExplosion);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

//Collision for exploding
void ASATORI_BlackHoleActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(OtherActor);

	if (!Character) 
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Explode();
	}
	if (!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		Explode();
	}
}

//Collision when exploding
void ASATORI_BlackHoleActor::OnOverlapSphereOnExplosion(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(OtherActor);

	if (Exploded) {
		if (Character->HasMatchingGameplayTag(EnemyTag)) {
			ArrayTrapped.AddUnique(Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()));
		}
	}
}

void ASATORI_BlackHoleActor::Explode() 
{

	GetWorldTimerManager().ClearTimer(TimerHandleDestroy);

	Exploded = true;

	if (IsValid(Controller)) {
		DisableInput(Controller);
	}

	Active = false;

	CollisionSphereComponent->SetSphereRadius(SphereRadiusOnExplosion, true);

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_BlackHoleActor::DestroyMyself, TimeToDestroy, false);

}

void ASATORI_BlackHoleActor::DestroyMyself()
{
	Destroy();
}

void ASATORI_BlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (IsValid(Controller)) {
		EnableInput(Controller);
	}

	if (!EnemyTag.IsValid() || !PlayerTag.IsValid() || !TargetActorWithTag.IsValid())
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

	Active = true;

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_BlackHoleActor::DestroyMyself, TimeToDestroy, false);

}

// Called every frame
void ASATORI_BlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();

	if (!Exploded)
	{
		//If has Target
		if (Target)
		{
			FVector TargetPosition = Target->GetActorLocation();
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ActorPosition, TargetPosition);
			SetActorLocation(ActorPosition + Direction * Speed * DeltaTime);
		}
		//If not has Target
		else
		{
			SetActorLocation(ActorPosition + GetActorForwardVector() * Speed * DeltaTime);
		}
	}

	if (Exploded) {
		for (UPrimitiveComponent* RootComp : ArrayTrapped) {
			FVector EnemyPos = RootComp->GetComponentLocation();
			RootComp->SetWorldLocation(EnemyPos + (ActorPosition - EnemyPos));
		}
	}
}


