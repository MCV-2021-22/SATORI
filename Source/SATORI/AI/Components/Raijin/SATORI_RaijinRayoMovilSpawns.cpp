#include "SATORI_RaijinRayoMovilSpawns.h"

#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DecalActor.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SATORI_RaijinRayoMovil.h"

ASATORI_RaijinRayoMovilSpawns::ASATORI_RaijinRayoMovilSpawns()
{

	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetSphereRadius(5.0f);

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponentInicio = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponentInicio->InitCapsuleSize(30.0f, 10.0f);
	CapsuleComponentInicio->SetupAttachment(RootComponent);
	CapsuleComponentInicio->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	RootComponent = CapsuleComponentInicio;
	


	CapsuleComponentFinal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Final"));
	CapsuleComponentFinal->InitCapsuleSize(30.0f, 10.0f);
	CapsuleComponentFinal->SetupAttachment(CapsuleComponentInicio);
	CapsuleComponentFinal->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponentFinal->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayoMovilSpawns::OnComponentBeginOverlapFinal);
	



	pfinal = CreateDefaultSubobject<ATargetPoint>(TEXT("Final2"));
	CapsuleComponentFinal->InitCapsuleSize(30.0f, 10.0f);
	CapsuleComponentFinal->SetupAttachment(RootComponent);
	
	//SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	//SphereComponent->SetCollisionProfileName(FName("Trigger"));
 	//SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentHit);
	//SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentBeginOverlap);
	//SphereComponent->SetSimulatePhysics(true);


	//RootComponent = SphereComponent;
	//RootComponent = CapsuleComponent;

	//StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMeshComponent->SetupAttachment(RootComponent);

	//Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	//Decal->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	
	//PSC->SetTemplate(PS.Object);


}


void ASATORI_RaijinRayoMovilSpawns::BeginPlay()
{
	/*
	
	PrimaryActorTick.bCanEverTick = true;

	*/
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Raijin.SpawnMovil");

	


}
void ASATORI_RaijinRayoMovilSpawns::OnComponentBeginOverlapFinal(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORI_RaijinRayoMovil* Rayo = Cast<ASATORI_RaijinRayoMovil>(OtherActor);
	if (Rayo && !Rayo->inmune )
	{

		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
		
		Rayo->destroyDecal();
		Rayo->Destroy();

		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);

	}




}

