#include "SATORI_RaijinRayo.h"

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

ASATORI_RaijinRayo::ASATORI_RaijinRayo()
{

	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetSphereRadius(5.0f);

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(300.0f, 1000.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayo::OnComponentBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_RaijinRayo::OnComponentEndOverlap);

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
	PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSC"));
	//PSC->SetTemplate(PS.Object);


	TArray< AActor* > enemigos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);


	UE_LOG(LogTemp, Display, TEXT("Bala creada 222 "));
}

void ASATORI_RaijinRayo::setDirection(FVector newDirection)
{

	this->direction = newDirection;
}


void ASATORI_RaijinRayo::BeginPlay()
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
		my_decal->GetDecal()->DecalSize = FVector(300.0f, 300.0f, 300.0f);
		//decal->SetLifeSpan(8);
		UE_LOG(LogTemp, Warning, TEXT("Yes decal spawned"));
		//m_previousActionDecal = decal;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}


}


void ASATORI_RaijinRayo::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	time_actual += DeltaTime;

	if(time_actual >= time_to_overlap)
	{
		CapsuleComponent->Activate(true);

		FVector RayoLocation = GetActorLocation();
	
		if(Trueno)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Trueno, RayoLocation);
		}
		
		if (Player && player_inside)
		{
			if (!Player->acti_rayo)
			{
				Player->acti_rayo = true;
				float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
				USATORI_BlueprintLibrary::ApplyGameplayEffect(Player, HitGameplayEffect);
			}

		}
		time_to_overlap = 100.0f;
		
	}
	/*else if (time_actual < time_to_destroy)
	{
		if(Player && player_inside)
		{
			if(!Player->acti_rayo)
			{
				Player->acti_rayo = true;
				float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
			}
			
		}
		
	}*/
	else if(time_actual >= time_to_destroy)
	{
		if(Player)
		{
			if(Player->singlerayo)
			{
				Player->acti_rayo = false;
			}
		}
		my_decal->Destroy();
		Destroy();
	}

	
}


void ASATORI_RaijinRayo::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);


	Destroy();

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

}



bool ASATORI_RaijinRayo::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
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

void ASATORI_RaijinRayo::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	if(Player1)
	{
		Player = Player1;
		player_inside = true;
		//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);

	}

	if(OtherActor->ActorHasTag(FName("LimitMap")))
	{
		Tags.Add("State.CollisionMapLimit");

	}


}

void ASATORI_RaijinRayo::OnComponentEndOverlap(
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

bool ASATORI_RaijinRayo::CheckCollision()
{

	if(ActorHasTag(FName("State.CollisionMapLimit")))
	{
		return true;

	}
	return false;

}