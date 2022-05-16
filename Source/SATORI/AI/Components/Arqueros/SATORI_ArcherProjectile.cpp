#include "SATORI_ArcherProjectile.h"

#include "SATORICharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

ASATORI_ArcherProjectile::ASATORI_ArcherProjectile()
{

	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(5.0f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	SphereComponent->SetSimulatePhysics(true);
	

	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);


	TArray< AActor* > enemigos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

	for (AActor* Actor : enemigos)
	{
		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORICharacter>(Actor) != nullptr)
		{
			ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

			FVector dest = Player->GetActorLocation();



			FVector newForward = dest - this->GetActorLocation();
			newForward.Normalize();

			//direction = newForward * 50;
			direction = FVector(1, 1, 0);
			direction.Normalize();
			direction = direction * 50;
			UE_LOG(LogTemp, Display, TEXT("Se esta creando la bala"));
			break;

		}

	}
	UE_LOG(LogTemp, Display, TEXT("Bala creada 222 "));
}

void setDirection(FVector newDirection)
{

	int a = 1;
}


void ASATORI_ArcherProjectile::BeginPlay()
{
	Super::BeginPlay();
	const bool bRecursive = true;

	TArray< AActor* > enemigos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);

	for (AActor* Actor : enemigos)
	{
		//Actor->Tags.Add("PossessedBy.Player");
		if (Cast<ASATORICharacter>(Actor) != nullptr)
		{
			ASATORICharacter* Player = Cast<ASATORICharacter>(Actor);
			bool tiene = Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

			UE_LOG(LogTemp, Display, TEXT("Number of actors with that tag: %d"), tiene);

			FVector dest = Player->GetActorLocation();



			FVector newForward = dest - this->GetActorLocation();
			newForward.Normalize();

			//direction = newForward * 50;
			direction = FVector(1, 1, 0);
			direction.Normalize();
			direction = direction * 50;

			break;

		}

	}

}


void ASATORI_ArcherProjectile::Tick(float DeltaTime)
{

	
	Super::Tick(DeltaTime);

	FVector position = GetActorLocation();
	FVector new_pos = position + direction;

	SetActorLocation(new_pos);
	RootComponent->SetWorldLocation(new_pos);

}


/*void ASATORI_ArcherProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

	//}



bool ASATORI_ArcherProjectile::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
{
	if (!InstigatorTagInterface || !OtherTagInterface) {
		return false;
	}

	// #TODO Use team agent interface
	FGameplayTag AITag = FGameplayTag::RequestGameplayTag("PossessedBy.AI");
	FGameplayTag PlayerTag = FGameplayTag::RequestGameplayTag("PossessedBy.Player");

	

	return (InstigatorTagInterface->HasMatchingGameplayTag(AITag) && OtherTagInterface->HasMatchingGameplayTag(PlayerTag) ) ||
		(InstigatorTagInterface->HasMatchingGameplayTag(PlayerTag) && OtherTagInterface->HasMatchingGameplayTag(AITag));

}

