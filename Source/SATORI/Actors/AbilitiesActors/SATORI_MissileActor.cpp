//

#include "Actors/AbilitiesActors/SATORI_MissileActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_MissileActor::ASATORI_MissileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	float SphereRadius = 32.0f;
	float SeekingSphereRadius = 256.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	//If collides will explode
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MissileActor::OnOverlapCollisionSphere);

	//If not targeting will Target first collision with it
	SeekingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SeekingSphere"));
	SeekingSphereComponent->SetSphereRadius(SeekingSphereRadius);
	SeekingSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreAllOverlapOnlyPawn")));
	SeekingSphereComponent->SetupAttachment(RootComponent);
	SeekingSphereComponent->SetGenerateOverlapEvents(true);
	SeekingSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_MissileActor::OnOverlapSeekingSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
	SeekingSphereComponent->bHiddenInGame = false;
}

//Collision for exploding
void ASATORI_MissileActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		DestroyMyself();
	}
	if (!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		DestroyMyself();
	}
}

//Collision for aiming
void ASATORI_MissileActor::OnOverlapSeekingSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (Character->HasMatchingGameplayTag(EnemyTag) && !Target)
	{
		Target = OtherActor;
	}
}

void ASATORI_MissileActor::DestroyMyself()
{
	Destroy();
}

void ASATORI_MissileActor::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyTag.IsValid() || !PlayerTag.IsValid() || !TargetActorWithTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_MissileActor: Tag is not valid ... "), *GetName());
	}


	//Check if Player is currently targeting an enemy
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorWithTag.GetTagName(), Actors);
	if (Actors.Num() != 0) 
	{
		Target = Actors.Pop();
	}

	//Set max time before auto destruc if not collides
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_MissileActor::DestroyMyself, TimeToDestroy, false);
	
}

void ASATORI_MissileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();
	//If has Target
	if(Target)
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

