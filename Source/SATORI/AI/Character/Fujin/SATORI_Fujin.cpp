// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Fujin/SATORI_Fujin.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
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
		CollisionL->SetCapsuleSize(30.f, 100.f, true);
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
		CollisionR->SetCapsuleSize(30.f, 100.f, true);
		//CollisionR->SetCollisionProfileName("Pawn");
		CollisionR->SetGenerateOverlapEvents(true);
		CollisionR->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnOverlapRight);
		CollisionR->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnEndOverlapRight);
		CollisionR->AttachTo(Right);
	}


	

	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Fujin/BT_Fujin.BT_Fujin")));
	posinicial = GetActorLocation();
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