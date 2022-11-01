#include "SATORI_FujinLevantarCollision.h"

#include "SATORICharacter.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "GameFramework/CharacterMovementComponent.h"


ASATORI_FujinLevantarCollision::ASATORI_FujinLevantarCollision()
{
	
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(10.0f);
	//SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	
	SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASATORI_FujinLevantarCollision::OnComponentHit);
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_FujinLevantarCollision::OnComponentBeginOverlap);
	//SphereComponent->SetSimulatePhysics(true);


	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}


void ASATORI_FujinLevantarCollision::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//SetActorEnableCollision(false);
	SphereComponent->SetCollisionProfileName(FName("NoCollision"));
	
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASATORI_FujinLevantarCollision::InmunityOff, inmunityTime, false);
	


}

void ASATORI_FujinLevantarCollision::InmunityOff()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	inmunity = false;
	SphereComponent->SetCollisionProfileName(FName("OverlapAll"));
	
	//SetActorEnableCollision(true);
	
}

void ASATORI_FujinLevantarCollision::ApplyDmg(ASATORICharacter* Player1)
{
	float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player1, Damage, Player1, DamageGameplayEffect);
	//SetActorEnableCollision(true);

}


void ASATORI_FujinLevantarCollision::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);


	Destroy();

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

}



bool ASATORI_FujinLevantarCollision::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
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

void ASATORI_FujinLevantarCollision::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	
	if(Player1 && !inmunity)
	{
			
			Player1->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.EndLevantado"));
			Destroy();
		
	}

	
}


void ASATORI_FujinLevantarCollision::PushPlayer(ASATORICharacter* Player1)
{
	Player = Player1;
	if (Player1->IsRootComponentMovable())
	{
		FVector Forward = this->GetActorForwardVector();

		FVector resultante = GetActorForwardVector() * ImpulseForce;

		//Player->GetMesh()->AddImpulse(FVector(100000000, 100000000,0));

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		Player1->DisableInput(PlayerController);

		//Player1->GetCharacterMovement()->AddImpulse(FVector(0, 0, ImpulseForce));
		Player1->GetCharacterMovement()->AddImpulse(FVector(resultante.X/5, resultante.Y/5, ImpulseForce/2));

		

	}


}


void ASATORI_FujinLevantarCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	FVector ActorPosition = GetActorLocation();
	FVector PlayerPosition = Player->GetActorLocation();

	FVector FinalPos = FVector(PlayerPosition.X, PlayerPosition.Y, ActorPosition.Z);
	
	SetActorLocation(FinalPos);

	

	//Stay grounded calculation
	ActorPosition = GetActorLocation();
	FVector End = ActorPosition;
	End.Z -= TraceDistanceToFloor;
	bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, ActorPosition, End, FName("BlockOnlyStatic"), CollisionParams);

	if (bHitAnything) {
		if (OutHit.Distance > MaxHeight) {
			ActorPosition.Z -= HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
		if (OutHit.Distance < MinHeight) {
			ActorPosition.Z += HeightChange * DeltaTime;
			SetActorLocation(ActorPosition);
		}
	}

	LifeTime -= DeltaTime;

	if(LifeTime <0)
	{
		if (Player )
		{

			Player->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.EndLevantado"));
			

		}
		Destroy();
		
	}

}