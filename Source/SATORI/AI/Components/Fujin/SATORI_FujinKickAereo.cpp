#include "SATORI_FujinKickAereo.h"

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
#include "AI/Character/Fujin/SATORI_Fujin.h"
#define ECC_Player ECollisionChannel::ECC_GameTraceChannel4
#define ECC_Enemy ECollisionChannel::ECC_GameTraceChannel5

ASATORI_FujinKickAereo::ASATORI_FujinKickAereo()
{

	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetSphereRadius(5.0f);

	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(300.0f, 400.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_FujinKickAereo::OnComponentBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_FujinKickAereo::OnComponentEndOverlap);

	RootComponent = CapsuleComponent;

	CapsuleComponentPeque = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponentPeque"));
	CapsuleComponentPeque->InitCapsuleSize(200.0f, 60.0f);
	CapsuleComponentPeque->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	CapsuleComponentPeque->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_FujinKickAereo::OnComponentBeginOverlapPeque);
	CapsuleComponentPeque->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_FujinKickAereo::OnComponentEndOverlapPeque);
	CapsuleComponentPeque->SetupAttachment(RootComponent);
	//SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	//SphereComponent->SetCollisionProfileName(FName("Trigger"));
	//SphereComponent->OnComponentHit.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentHit);
	//SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_ArcherProjectile::OnComponentBeginOverlap);
	//SphereComponent->SetSimulatePhysics(true);


	//RootComponent = SphereComponent;
	

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

void ASATORI_FujinKickAereo::setDirection(FVector newDirection)
{

	this->direction = newDirection;
}


void ASATORI_FujinKickAereo::BeginPlay()
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


void ASATORI_FujinKickAereo::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	time_actual += DeltaTime;

	/*if (time_actual >= time_to_overlap)
	{
		CapsuleComponent->Activate(true);

		FVector RayoLocation = GetActorLocation();
	
		if(Trueno)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Trueno, RayoLocation);
		}
		
		
		time_to_overlap = 100.0f;
		
	}
	else if(time_actual < time_to_destroy)
	{
		if(Player)
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
		}
		
	}
	else if(time_actual >= time_to_destroy)
	{
		my_decal->Destroy();
		Destroy();
	}*/

	
}


void ASATORI_FujinKickAereo::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* InstigatorPawn = GetInstigator<APawn>();
	IGameplayTagAssetInterface* InstigatorTagInterface = Cast<IGameplayTagAssetInterface>(InstigatorPawn);
	IGameplayTagAssetInterface* OtherTagInterface = Cast<IGameplayTagAssetInterface>(OtherActor);


	Destroy();

	/*if (IsHostile(InstigatorTagInterface, OtherTagInterface)) {
		HandleHit(OtherActor);
	}*/

}



bool ASATORI_FujinKickAereo::IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const
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

void ASATORI_FujinKickAereo::OnComponentBeginOverlap(
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

	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(OtherActor);
	if (Fujin)
	{
		int a = 1;
		//Fujin->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
		//Fujin->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		if(Player)
		{
			Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}
		


		/*Fujin_inside = true;
		if(player_inside)
		{
			Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Fujin.PlayerHitted"));
			Fujin->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
			//Fujin->SetActorEnableCollision(false);
			
			
		}
		Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Fujin.KickDone"));
		*/
		


		//my_decal->Destroy();
		//Destroy();

	}

	


}

void ASATORI_FujinKickAereo::OnComponentEndOverlap(
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

bool ASATORI_FujinKickAereo::CheckCollision()
{

	if(ActorHasTag(FName("State.CollisionMapLimit")))
	{
		return true;

	}
	return false;

}


void ASATORI_FujinKickAereo::OnComponentBeginOverlapPeque(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASATORI_Fujin* Fujin = Cast<ASATORI_Fujin>(OtherActor);
	if (Fujin)
	{
		Fujin_inside = true;
		if (player_inside)
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, Damage, Player, DamageGameplayEffect);
			Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Fujin.PlayerHitted"));
			//Fujin->GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
			//Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			
			//my_decal->Destroy();
			//Destroy();
			//Fujin->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			//float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
			//Fujin->SetActorEnableCollision(false);


		}
		Fujin->AddGameplayTag(FGameplayTag::RequestGameplayTag("Fujin.KickDone"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASATORI_FujinKickAereo::enableCollision, 0.5f, false);

	}
	

		
		/*Fujin->GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
		
		my_decal->Destroy();
		Destroy();*/

	




}

void ASATORI_FujinKickAereo::OnComponentEndOverlapPeque(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	

}

void ASATORI_FujinKickAereo::setCollisionFujin()
{
	//FujinVar->GetCapsuleComponent()->SetCollisionProfileName(FName("OverlapAll"));
	//FujinVar->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);
	if(Player)
	{
		Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	

	
}


void ASATORI_FujinKickAereo::enableCollision()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//FujinVar->GetCapsuleComponent()->SetCollisionProfileName(FName("Pawn"));
	if(Player)
	{
		Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	my_decal->Destroy();
	Destroy();
	//FujinVar->SetActorEnableCollision(true);


}
