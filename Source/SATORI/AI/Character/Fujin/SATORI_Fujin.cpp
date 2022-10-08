// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Fujin/SATORI_Fujin.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"


ASATORI_Fujin::ASATORI_Fujin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	//CharacterMovement->bRunPhysicsWithNoController = true;

	//CharMoveComp

	//uint8 bRunPhysicsWithNoController : 1;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//CharacterMovement->bRunPhysicsWithNoController = 3;


	Left = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left"));
	CollisionL = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Left Collision"));
	if (Left)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Left->AttachToComponent(GetMesh(), AttachmentRules, "LHand");
		// Sphere Collision
		CollisionL->SetCapsuleSize(30.f, 50.f, true);
		//CollisionL->SetCollisionProfileName("Pawn");
		CollisionL->SetGenerateOverlapEvents(true);
		CollisionL->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnOverlapLeft);
		CollisionL->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnEndOverlapLeft);
		CollisionL->AttachTo(Left);
	}

	Right = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right"));
	CollisionR = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Right Collision"));
	if (Right)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Right->AttachToComponent(GetMesh(), AttachmentRules, "RHand");
		// Sphere Collision
		CollisionR->SetCapsuleSize(30.f, 50.f, true);
		//CollisionR->SetCollisionProfileName("Pawn");
		CollisionR->SetGenerateOverlapEvents(true);
		CollisionR->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnOverlapRight);
		CollisionR->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnEndOverlapRight);
		CollisionR->AttachTo(Right);
	}

	

	//{
		//const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		//Nube->AttachToComponent(GetMesh(), AttachmentRules, "BckNubeFujin");

		//UNiagaraFunctionLibrary::SpawnSystemAttached(Nube,GetMesh(), "BckNubeFujin", AttachmentRules);
	//}

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Trueno, RayoLocation);

	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Fujin/BT_Fujin.BT_Fujin")));
	posinicial = GetActorLocation();
}


void ASATORI_Fujin::BeginPlay()
{
	Super::BeginPlay();

	//(UNiagaraSystem * SystemTemplate, USceneComponent * AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, ENCPoolMethod PoolingMethod, bool bAutoActivate = true, bool bPreCullCheck = true);
	Nube1 = UNiagaraFunctionLibrary::SpawnSystemAttached(Nube, GetMesh(), TEXT("BckNubeFujin"), FVector(0), FRotator::ZeroRotator,FVector(0.1f,0.1f,0.1f), EAttachLocation::Type::KeepRelativeOffset, false,  ENCPoolMethod::None);
	
	/*if (Nube1)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Nube1->AttachToComponent(GetMesh(), AttachmentRules, "BckNubeFujin");
	}*/

	CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASATORI_Fujin::setRaijin, 0.5f, false);



}
void ASATORI_Fujin::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);
	
	

}

float ASATORI_Fujin::getDistAttack()
{
	return dist_attack;
}



float ASATORI_Fujin::getCloseDist()
{
	return close_dist;
}




void ASATORI_Fujin::OnOverlapLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);

	if (!Character)
	{
		canMove = false;
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Left"));

	}

}

void ASATORI_Fujin::OnOverlapRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);

	if (!Character)
	{
		canMove = false;
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockRight"));

	}

}

void ASATORI_Fujin::OnEndOverlapLeft(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);

	if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockLeft"));
	}

}


void ASATORI_Fujin::OnEndOverlapRight(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);

	if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockRight"));
	}

}


void ASATORI_Fujin::setRaijin()
{
	TSubclassOf<ASATORI_Raijin> classToFind;
	classToFind = ASATORI_Raijin::StaticClass();

	TArray< AActor* > enemigos;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, enemigos);

	if (enemigos.Num() > 0)
	{
		ASATORI_Raijin* Raij = Cast<ASATORI_Raijin>(enemigos[0]);
		if (Raij)
		{
			Raijin = Raij;
		}

	}

}


bool ASATORI_Fujin::getDowned()
{
	return downed;
}


void ASATORI_Fujin::setDowned(bool dw)
{
	downed = dw;


}

void ASATORI_Fujin::startCDDowned()
{

	setDowned(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleDowned, this, &ASATORI_Fujin::revivirTag, CdTimeDowned, false);

}

void ASATORI_Fujin::revivirTag()
{
	RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Downed"));

}


void ASATORI_Fujin::revivir()
{
	setDowned(false);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleDowned);

	SetHealth(GetMaxHealth() * 0.25f);

}

bool ASATORI_Fujin::getRaijinDowned()
{
	return Raijin->getDowned();
}

