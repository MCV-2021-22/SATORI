#include "SATORI_RaijinRayoMovilSpawns.h"

#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DecalActor.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"

ASATORI_RaijinRayoMovilSpawns::ASATORI_RaijinRayoMovilSpawns()
{

	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetSphereRadius(5.0f);

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponentInicio = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponentInicio->InitCapsuleSize(1.0f, 1.0f);
	CapsuleComponentInicio->SetupAttachment(RootComponent);
	CapsuleComponentInicio->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);


	


	CapsuleComponentFinal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Final"));
	CapsuleComponentFinal->InitCapsuleSize(1.0f, 10.0f);
	CapsuleComponentFinal->SetupAttachment(RootComponent);
	CapsuleComponentFinal->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	
	

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


