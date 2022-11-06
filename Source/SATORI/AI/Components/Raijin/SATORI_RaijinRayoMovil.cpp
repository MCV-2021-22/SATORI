#include "SATORI_RaijinRayoMovil.h"
#include "NiagaraComponent.h"
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
#include "SATORI_RaijinRayoMovilSpawns.h"

ASATORI_RaijinRayoMovil::ASATORI_RaijinRayoMovil()
{

	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetSphereRadius(5.0f);

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(decal_size, 1000.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayoMovil::OnComponentBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayoMovil::OnComponentEndOverlap);

	


	CapsuleComponentFinal = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Final"));
	CapsuleComponentFinal->InitCapsuleSize(30.0f, 100.0f);
	CapsuleComponentFinal->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponentFinal->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayoMovil::OnComponentBeginOverlapFinal);
	

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

}

void ASATORI_RaijinRayoMovil::setDirection(FVector newDirection)
{

	this->direction = newDirection;
}


void ASATORI_RaijinRayoMovil::BeginPlay()
{
	/*
	
	PrimaryActorTick.bCanEverTick = true;

	*/
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	//ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(GetActorLocation(), FRotator());

	//my_decal = decal;
	/*if (my_decal)
	{
		my_decal->SetDecalMaterial(MaterialDecal);
		my_decal->SetLifeSpan(0);
		my_decal->GetDecal()->DecalSize = FVector(decal_size, decal_size, decal_size);
		//decal->SetLifeSpan(8);
		//m_previousActionDecal = decal;
	}*/
	if (Trueno)
	{
		FVector RayoLocation = GetActorLocation()- FVector(0,0,500);
		ParticleEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Trueno, RayoLocation, FRotator(90.0, 0, 0));
	}

}


void ASATORI_RaijinRayoMovil::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if(timeiNMUNE>0)
	{
		timeiNMUNE -= DeltaTime;
	}else
	{
		inmune = false;
	}

	//Velocity
	
	FVector NewPos = GetActorLocation() + direction * velocity * 1.5;
	//FVector NewPos = GetActorLocation() + direction * velocity * 0.2;
	
	SetActorRelativeLocation(NewPos);
	//SetActorLocation(NewPos);

	/*if (my_decal)
	{
		my_decal->SetActorLocation(NewPos);

		
		
	}*/
	if(ParticleEffect)
	{
		
			if(ParticleEffect->IsActive())
			{
				ParticleEffect->SetRelativeLocation(NewPos);
			}
		
	}
	
	
	time_to_dmg -= DeltaTime;

	if(time_to_dmg<=0.0f)
	{
		time_to_dmg = time_to_dmg_max;
		if (Player && player_inside)
		{

			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(Player, HitGameplayEffect);


		}
	}

	LifeTime -= DeltaTime;
	if(LifeTime<=0)
	{
		if (ParticleEffect)
		{
			if (ParticleEffect->IsActive())
			{
				ParticleEffect->DestroyComponent();
			}
		}
		destruido = true;
	}
	
	
}


void ASATORI_RaijinRayoMovil::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);


	//Destroy();

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

}



bool ASATORI_RaijinRayoMovil::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
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

void ASATORI_RaijinRayoMovil::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	if (Player1)
	{
		Player = Player1;
		player_inside = true;
		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);

	}

	ASATORI_RaijinRayoMovilSpawns* Rayo = Cast<ASATORI_RaijinRayoMovilSpawns>(OtherActor);
	if (Rayo && !inmune)
	{

		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);

		//my_decal->Destroy();
		if (ParticleEffect)
		{
			if (ParticleEffect->IsActive())
			{
				ParticleEffect->DestroyComponent();
			}
		}
		
		

		destruido = true;
		//Destroy();

		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);

	}

}

void ASATORI_RaijinRayoMovil::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	if (Player1)
	{
		//Player = Player1;
		player_inside = false;


	}

}

void ASATORI_RaijinRayoMovil::OnComponentBeginOverlapFinal(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORI_RaijinRayoMovilSpawns* Rayo = Cast<ASATORI_RaijinRayoMovilSpawns>(OtherActor);
	if (Rayo && !inmune)
	{

		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);

		//my_decal->Destroy();
		if (ParticleEffect)
		{
			if (ParticleEffect->IsActive())
			{
				ParticleEffect->DestroyComponent();
			}
		}

		destruido = true;
		//Destroy();

		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);

	}


	
	

}

void ASATORI_RaijinRayoMovil::destroyDecal()
{
	//my_decal->Destroy();
	if (ParticleEffect)
	{
		if (ParticleEffect->IsActive())
		{
			ParticleEffect->DestroyComponent();
		}
	}

}




bool ASATORI_RaijinRayoMovil::getDestruido()
{
	return destruido;
}