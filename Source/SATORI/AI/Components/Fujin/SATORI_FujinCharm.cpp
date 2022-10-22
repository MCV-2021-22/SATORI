#include "SATORI_FujinCharm.h"

#include "SATORICharacter.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"

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

	HitAreaComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitAreaComponent"));
	HitAreaComponent->SetupAttachment(RootComponent);


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

	if(!GameInstanceRef->TimeStop && !ReturnToFujinDone)
	{
		if(GameInstanceRef->TimeSlow)
		{
			FVector position = GetActorLocation();
			FVector new_pos = position + direction * 0.25;

			SetActorLocation(new_pos);
			//RootComponent->SetWorldLocation(new_pos);

			inmunity -= DeltaTime * 0.25;
			LifeTime -= DeltaTime * 0.25;
		}
		else
		{
			FVector position = GetActorLocation();
			FVector new_pos = position + direction;

			SetActorLocation(new_pos);
			//RootComponent->SetWorldLocation(new_pos);

			inmunity -= DeltaTime;
			LifeTime -= DeltaTime;
		}


		
	}
	if(ReturnToFujin && !ReturnToFujinDone)
	{
		Player->SetActorLocation(GetActorLocation());
	}

	if(canDestroy)
	{
		Destroy();
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

	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	if(Player1 && !ReturnToFujin && Fujin)
	{
		ReturnToFujin = true;

		// Particles
		FVector SpawnLocation = HitAreaComponent->GetComponentLocation();

		if (Charm_Hit_Particle)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Charm_Hit_Particle, SpawnLocation);
		}


		//SetActorHiddenInGame(true);
		StaticMeshComponent->SetVisibility(false);
		
		FVector dest = Fujin->GetActorLocation();
		//FVector dest = FVector(0, 0, 0);
		//float dmg_done= USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, damage, OtherActor, DamageGameplayEffect);

		FVector newForward = dest - GetActorLocation();
		newForward.Normalize();
		
		setDirection(newForward * 60);
		Player = Player1;

		Player->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed"));

	}
	if(ReturnToFujin)
	{
		ASATORI_Fujin* Fujin1 = Cast<ASATORI_Fujin>(OtherActor);

		if(Fujin1)
		{
			Player->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed"));
			ReturnToFujinDone = true;
			//Destroy();
		}
	}


	
	
}