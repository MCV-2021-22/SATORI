//

#include "AI/Components/Raijin/SATORI_RaijinDefensePush.h"
#include "Components/SphereComponent.h"
#include "SATORI/AI/Character/SATORI_AICharacter.h"
#include "SATORI/FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "SATORICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASATORI_RaijinDefensePush::ASATORI_RaijinDefensePush()
{
	PrimaryActorTick.bCanEverTick = true;

	float SphereRadius = 32.0f;

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionSphereComponent->SetSphereRadius(SphereRadius);
	CollisionSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	RootComponent = CollisionSphereComponent;
	CollisionSphereComponent->SetGenerateOverlapEvents(true);
	CollisionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_RaijinDefensePush::OnOverlapSphere);

	//Debug
	//CollisionSphereComponent->bHiddenInGame = false;
	ImpulseForce = 10000000.0f;
}

void ASATORI_RaijinDefensePush::OnOverlapSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{

	/*ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character) 
	{
		Destroy();
		return;
	}

	if(Character->HasMatchingGameplayTag(EnemyTag))
	{	
		float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(OtherActor, Damage, OtherActor, DamageGameplayEffect);
		Character->sendDamage(dmg_done);
		ArrayPushed.AddUnique(Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()));
	}
	if(!Character->HasMatchingGameplayTag(PlayerTag) && !Character->HasMatchingGameplayTag(EnemyTag))
	{
		Destroy();
	}*/
}

void ASATORI_RaijinDefensePush::DestroyMyself()
{
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//Player1->DisableInput(PlayerController);
	Destroy();
}

void ASATORI_RaijinDefensePush::BeginPlay()
{

	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	

	//Set max time before auto destruc if not collides

	ImpulseForce = 500000.0f;

	GetWorldTimerManager().SetTimer(TimerHandleDestroy, this, &ASATORI_RaijinDefensePush::DestroyMyself, TimeToDestroy, false);
}

void ASATORI_RaijinDefensePush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FHitResult Hit;

	const FVector Start = GetActorLocation();
	FVector End = Start + GetActorRotation().Vector() * TraceDistance;

	FCollisionQueryParams TraceParams*/


	
}

void ASATORI_RaijinDefensePush::PushPlayer(ASATORICharacter* Player)
{

	if(Player->IsRootComponentMovable())
	{
		FVector Forward = this->GetActorForwardVector();

		FVector resultante = GetActorForwardVector() * ImpulseForce ;

		//Player->GetMesh()->AddImpulse(FVector(100000000, 100000000,0));
		Player1 = Player;
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		Player->DisableInput(PlayerController);
		Player->GetCharacterMovement()->AddImpulse(FVector(resultante.X / 5, resultante.Y / 5, ImpulseForce / 5));

		

	}


}
