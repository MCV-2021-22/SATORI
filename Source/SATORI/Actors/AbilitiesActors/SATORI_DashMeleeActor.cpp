//

#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "SATORICharacter.h"
#include "AI/Character/Melee/SATORI_Melee.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

ASATORI_DashMeleeActor::ASATORI_DashMeleeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->SetupAttachment(RootComponent);
	CollisionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapEnd);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapSphere);

	MeleeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereMelee"));
	MeleeSphereComponent->SetSphereRadius(SphereRadius);
	MeleeSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	MeleeSphereComponent->SetGenerateOverlapEvents(true);
	MeleeSphereComponent->SetupAttachment(RootComponent);
	MeleeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DashMeleeActor::OnOverlapSphereMelee);


	//Debug
	CollisionSphereComponent->bHiddenInGame = false;
}

void ASATORI_DashMeleeActor::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{

	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if (Character)
	{
		DamagePlayer = true;
		Damage = 10.f;
	}
	else
	{
		ASATORI_Melee* Melee = Cast<ASATORI_Melee>(OtherActor);
		if (Melee)
		{
			if(USkeletalMeshComponent* Mesh = Melee->GetMesh())
			{
				if(UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
				{
					AnimInstance->Montage_JumpToSection(FName("ending"), AnimInstance->GetCurrentActiveMontage());
				}
			}
		}
	}
}

void ASATORI_DashMeleeActor::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if(Character)
	{
		DamagePlayer = false;
	}
}

void ASATORI_DashMeleeActor::OnOverlapSphereMelee(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	ASATORI_Melee* Melee = Cast<ASATORI_Melee>(OtherActor);

	if(Melee)
	{
		Melee->AddGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop"));

		
		DestroyObject = true;
		
	}
}

void ASATORI_DashMeleeActor::DestroyMyself()
{
	my_decal->Destroy();
	Destroy();
}

void ASATORI_DashMeleeActor::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(GetActorLocation(), FRotator());

	my_decal = decal;
	if (my_decal)
	{
		my_decal->SetDecalMaterial(MaterialDecal);
		my_decal->SetLifeSpan(0);
		my_decal->GetDecal()->DecalSize = FVector(300.0f, 300.0f, 300.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
	}
}

void ASATORI_DashMeleeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(DestroyObject)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if(Character && DamagePlayer)
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Character, Damage, Character, DamageGameplayEffect);
		}
		DestroyMyself();
	}

	if(CurrentTime >= 20.f)
	{
		DestroyMyself();
	}

	CurrentTime += DeltaTime;

}