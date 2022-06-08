//

#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "SATORICharacter.h"
#include "AI/Character/Melee/SATORI_Melee.h"

ASATORI_DashMeleeActor::ASATORI_DashMeleeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->SetupAttachment(RootComponent);

	MeleeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereMelee"));
	MeleeSphereComponent->SetSphereRadius(SphereRadius);
	MeleeSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	MeleeSphereComponent->SetGenerateOverlapEvents(true);
	MeleeSphereComponent->SetupAttachment(RootComponent);
	MeleeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapSphereMelee);


	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_DashMeleeActor::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{

	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if (Character)
	{
		float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
	}
}

void ASATORI_DashMeleeActor::OnOverlapSphereMelee(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	ASATORI_Melee* Melee = Cast<ASATORI_Melee>(OtherActor);

	if(Melee)
	{
		CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapSphere);
		Melee->AddGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop"));
	}
}

void ASATORI_DashMeleeActor::DestroyMyself()
{	
	Destroy();
}

void ASATORI_DashMeleeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

	if(CurrentTime >= 20.f)
	{
		DestroyMyself();
	}

}

