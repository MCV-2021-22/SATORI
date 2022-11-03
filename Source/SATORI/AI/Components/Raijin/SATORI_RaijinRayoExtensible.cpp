#include "SATORI_RaijinRayoExtensible.h"

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

ASATORI_RaijinRayoExtensible::ASATORI_RaijinRayoExtensible()
{

	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetSphereRadius(5.0f);

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(decal_size, 1000.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayoExtensible::OnComponentBeginOverlap);
	

	//SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	//SphereComponent->SetCollisionProfileName(FName("Trigger"));
 	//SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentHit);
	//SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentBeginOverlap);
	//SphereComponent->SetSimulatePhysics(true);


	//RootComponent = SphereComponent;
	RootComponent = CapsuleComponent;

	//StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMeshComponent->SetupAttachment(RootComponent);

	//Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	//Decal->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	
	//PSC->SetTemplate(PS.Object);


	TArray< AActor* > enemigos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);


	UE_LOG(LogTemp, Display, TEXT("Bala creada 222 "));
}

void ASATORI_RaijinRayoExtensible::setDirection(FVector newDirection)
{

	this->direction = newDirection;
}


void ASATORI_RaijinRayoExtensible::BeginPlay()
{
	/*
	
	PrimaryActorTick.bCanEverTick = true;

	*/
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(GetActorLocation(), FRotator());

	my_decal = decal;
	if (my_decal)
	{
		my_decal->SetDecalMaterial(MaterialDecal);
		my_decal->SetLifeSpan(0);
		my_decal->GetDecal()->DecalSize = FVector(decal_size, decal_size, decal_size);
		//decal->SetLifeSpan(8);
		UE_LOG(LogTemp, Warning, TEXT("Yes decal spawned"));
		//m_previousActionDecal = decal;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}

	


}


void ASATORI_RaijinRayoExtensible::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	time_to_destroy -= DeltaTime;

	if(time_to_destroy >= 0)
	{
		CapsuleComponent->Activate(true);

		

		decal_size += DeltaTime;
		scale += DeltaTime;
		//CapsuleComponent->SetCapsuleSize(decal_size, 1000.0f);
		CapsuleComponent->SetRelativeScale3D(FVector(scale, scale, scale));
		//my_decal->GetDecal()->DecalSize = FVector(decal_size, decal_size, decal_size);
		my_decal->GetDecal()->SetRelativeScale3D(FVector(scale, scale, scale));
	}
	else
	{
		my_decal->Destroy();
		destruible = true;
		//Destroy();
	}
	

	
}


void ASATORI_RaijinRayoExtensible::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);


	Destroy();

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

}



bool ASATORI_RaijinRayoExtensible::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
{
	if (!InstigatorTagInterface || !OtherTagInterface) {
		return false;
	}

	// #TODO Use team agent interface
	FGameplayTag AITag = FGameplayTag::RequestGameplayTag("PossessedBy.AI");
	FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("PossessedBy.Player");



	return (InstigatorTagInterface->HasMatchingGameplayTag(AITag) && OtherTagInterface->HasMatchingGameplayTag(PlayerTag)) ||
		(InstigatorTagInterface->HasMatchingGameplayTag(PlayerTag) && OtherTagInterface->HasMatchingGameplayTag(AITag));

}

void ASATORI_RaijinRayoExtensible::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);
	if(Player)
	{

		float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
		USATORI_BlueprintLibrary::ApplyGameplayEffect(Player, HitGameplayEffect);

		my_decal->Destroy();

		FVector RayoLocation = Player->GetActorLocation();

		if (Trueno)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Trueno, RayoLocation);
		}
		//Destroy();
		destruible = true;
		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);

	}

}

