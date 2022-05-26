//

#include "Actors/AbilitiesActors/SATORI_DecoyActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_DecoyActor::ASATORI_DecoyActor()
{
	PrimaryActorTick.bCanEverTick = false;

	float SphereRadiusOfAction = 600.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	//If collides will lure enemies
	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadiusOfAction);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DecoyActor::OnOverlapCollisionSphere);

	//Debug
	CollisionSphereComponent->bHiddenInGame = false;

}

//Collision for luring
void ASATORI_DecoyActor::OnOverlapCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);
	
	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->AddGameplayTag(TagGrantedWhenLured);
		ArrayLured.AddUnique(OtherActor);
	}
}

void ASATORI_DecoyActor::DestroyMyself()
{
	for (AActor* Actor : ArrayLured)
	{
		ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);
		Character->RemoveGameplayTag(TagGrantedWhenLured);
	}
	Destroy();
}


void ASATORI_DecoyActor::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyTag.IsValid() || !PlayerTag.IsValid() || !TargetActorWithTag.IsValid() || !TagGrantedWhenLured.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_DecoyActor: Tag is not valid ... "), *GetName());
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorWithTag.GetTagName(), Actors);
	if (Actors.Num() != 0)
	{
		Target = Actors.Pop();
	}
	
	//Set max time before auto destruc if not collides
	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_DecoyActor::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_DecoyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

