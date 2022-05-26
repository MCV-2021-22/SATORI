
#include "Actors/AbilitiesActors/SATORI_PullActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

// Sets default values
ASATORI_PullActor::ASATORI_PullActor()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;
	float SeekingSphereRadius = 256.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	//If collides will grab
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PullActor::OnOverlapCollisionSphere);

	//If not targeting will Target first collision with it
	SeekingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SeekingSphere"));
	SeekingSphereComponent->SetSphereRadius(SeekingSphereRadius);
	SeekingSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreAllOverlapOnlyPawn")));
	SeekingSphereComponent->SetupAttachment(RootComponent);
	SeekingSphereComponent->SetGenerateOverlapEvents(true);
	SeekingSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_PullActor::OnOverlapSeekingSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
	SeekingSphereComponent->bHiddenInGame = false;

}

//Collision for Grabing
void ASATORI_PullActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Pulling = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		GetWorldTimerManager().ClearTimer(TimerHandleDestroy);
	}
	if (!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		DestroySelf();
	}
}

//Collision for aiming
void ASATORI_PullActor::OnOverlapSeekingSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (Character->HasMatchingGameplayTag(EnemyTag) && !Target)
	{
		Target = OtherActor;
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
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorWithTag.GetTagName(), Actors);
	if (Actors.Num() != 0) {
		Target = Actors.Pop();
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

		FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ActorPosition, PlayerPosition);
		SetActorLocation(ActorPosition + Direction * SpeedPulling * DeltaTime);

		ActorPosition = GetActorLocation();
		Pulling->SetWorldLocation(ActorPosition);

		//If has reached the player
		if (FVector::Dist(PlayerPosition, ActorPosition) < 100) {
			DestroySelf();
		}
	}
	//Movement forward
	else
	{	
		//If has target
		if (Target)
		{
			FVector TargetPosition = Target->GetActorLocation();
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ActorPosition, TargetPosition);
			SetActorLocation(ActorPosition + Direction * SpeedForward * DeltaTime);
		}
		//If not has Target
		else
		{
			SetActorLocation(ActorPosition + GetActorForwardVector() * SpeedForward * DeltaTime);
		}
	}
}

void ASATORI_PullActor::DestroySelf()
{
	Destroy();
}
