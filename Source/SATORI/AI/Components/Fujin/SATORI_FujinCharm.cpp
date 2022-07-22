#include "SATORI_FujinCharm.h"

#include "SATORICharacter.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

ASATORI_FujinCharm::ASATORI_FujinCharm()
{

	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(5.0f);
	//SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	
	SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASATORI_FujinCharm::OnComponentHit);
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_FujinCharm::OnComponentBeginOverlap);
	//SphereComponent->SetSimulatePhysics(true);


	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);


	TArray< AActor* > enemigos;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);


	UE_LOG(LogTemp, Display, TEXT("Bala creada 222 "));
}

void ASATORI_FujinCharm::setDirection(FVector newDirection)
{

	this->direction = newDirection;
}


void ASATORI_FujinCharm::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	

}


void ASATORI_FujinCharm::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if(!GameInstanceRef->TimeStop)
	{
		if(GameInstanceRef->TimeSlow)
		{
			FVector position = GetActorLocation();
			FVector new_pos = position + direction * 0.25;

			SetActorLocation(new_pos);
			RootComponent->SetWorldLocation(new_pos);

			inmunity -= DeltaTime * 0.25;
			LifeTime -= DeltaTime * 0.25;
		}
		else
		{
			FVector position = GetActorLocation();
			FVector new_pos = position + direction;

			SetActorLocation(new_pos);
			RootComponent->SetWorldLocation(new_pos);

			inmunity -= DeltaTime;
			LifeTime -= DeltaTime;
		}


		
	}

	
}


void ASATORI_FujinCharm::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);


	Destroy();

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

}



bool ASATORI_FujinCharm::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
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

void ASATORI_FujinCharm::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	ASATORI_FujinCharm* Choque = Cast<ASATORI_FujinCharm>(OtherActor);

	if(Choque)
	{
		ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);
	}
	else if (!Choque || inmunity <= 0 || LifeTime <= 0)
	{
		ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);
		if(Player)
		{
			float dmg_done= USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, damage, OtherActor, DamageGameplayEffect);
		}
		Destroy();
	}
}