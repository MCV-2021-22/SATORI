// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectileSpawnerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"


ASATORI_RangeMovable::ASATORI_RangeMovable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	//CharacterMovement->bRunPhysicsWithNoController = true;

	//CharMoveComp

	//uint8 bRunPhysicsWithNoController : 1;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//CharacterMovement->bRunPhysicsWithNoController = 3;


	

	
	//AttackingRangeMovableB->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMCV_Pickup::OnComponentBeginOverlap);

	// Weapon Component
	Back = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Back"));
	CollisionB = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Back Collision"));
	if (Back)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Back->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket2");
		// Sphere Collision
		CollisionB->SetCapsuleSize(30.f, 100.f, true);
		//CollisionB->SetCollisionProfileName("Pawn");
		CollisionB->SetGenerateOverlapEvents(true);
		CollisionB->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RangeMovable::OnOverlapBehind);
		CollisionB->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_RangeMovable::OnEndOverlapBehind);
		CollisionB->AttachTo(Back);
	}

	Left = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left"));
	CollisionL = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Left Collision"));
	if (Left)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Left->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket2");
		// Sphere Collision
		CollisionL->SetCapsuleSize(30.f, 100.f, true);
		//CollisionL->SetCollisionProfileName("Pawn");
		CollisionL->SetGenerateOverlapEvents(true);
		CollisionL->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RangeMovable::OnOverlapLeft);
		CollisionL->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_RangeMovable::OnEndOverlapLeft);
		CollisionL->AttachTo(Left);
	}

	Right = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right"));
	CollisionR = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Right Collision"));
	if (Right)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Right->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket2");
		// Sphere Collision
		CollisionR->SetCapsuleSize(30.f, 100.f, true);
		//CollisionR->SetCollisionProfileName("Pawn");
		CollisionR->SetGenerateOverlapEvents(true);
		CollisionR->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_RangeMovable::OnOverlapRight);
		CollisionR->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_RangeMovable::OnEndOverlapRight);
		CollisionR->AttachTo(Right);
	}



	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/RangeMovable/BT_RangeMovable.BT_RangeMovable")));
	posinicial = GetActorLocation();
}

void ASATORI_RangeMovable::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);
	float Value = 1.0f;


	if ((Controller != nullptr) && (Value != 0.0f) && moveBackwards)
	{

		
		
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);


		
			if(canMove)
			{
				AddMovementInput(Direction, -Value, false);
			}
			
		

	}
	else if ((Controller != nullptr) && (Value != 0.0f) && moveLeft) {
		// find out which way is forward
		TArray< AActor* > enemigos;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		for (AActor* Actor : enemigos)
		{
			if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
			{

				ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);

				FVector fwd = Player->GetActorLocation() - GetActorLocation();

				FVector dest1 = GetActorLocation() + fwd;

				FVector RotateValue2 = fwd.RotateAngleAxis(15.0f, FVector(0, 0, 1));
				
				FVector dest2 = dest1 - RotateValue2;

				FVector dest3 = dest2 - GetActorLocation();

				
				if (canMove)
				{
					AddMovementInput(dest3, Value);
				}
				
				break;

			}
		}

		
	}
	else if ((Controller != nullptr) && (Value != 0.0f) && moveRight)
	{
		// find out which way is forward
		TArray< AActor* > enemigos;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PossessedBy.Player"), enemigos);
		for (AActor* Actor : enemigos)
		{
			if (Cast<ASATORI_CharacterBase>(Actor) != nullptr)
			{

				ASATORI_CharacterBase* Player = Cast<ASATORI_CharacterBase>(Actor);

				FVector fwd = Player->GetActorLocation() - GetActorLocation();

				FVector dest1 = GetActorLocation() + fwd;

				FVector RotateValue2 = fwd.RotateAngleAxis(-15.0f, FVector(0, 0, 1));
				
				FVector dest2 = dest1 - RotateValue2;

				FVector dest3 = dest2 - GetActorLocation();

				if (canMove)
				{
					AddMovementInput(dest3, Value);
				}
				

				break;

			}
		}
	}

}

float ASATORI_RangeMovable::getDistAttack()
{
	return dist_attack;
}



float ASATORI_RangeMovable::getCloseDist()
{
	return close_dist;
}

void ASATORI_RangeMovable::OnOverlapBehind(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(OtherActor);

	if(!Character)
	{
		canMove = false;
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockBehind"));
		
	}
	
	
}
void ASATORI_RangeMovable::OnOverlapLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(OtherActor);

	if (!Character)
	{
		canMove = false;
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Left"));

	}

}

void ASATORI_RangeMovable::OnOverlapRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(OtherActor);

	if (!Character)
	{
		canMove = false;
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockRight"));

	}

}
void ASATORI_RangeMovable::OnEndOverlapBehind(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(OtherActor);

	if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockBehind"));
	}
	

}
void ASATORI_RangeMovable::OnEndOverlapLeft(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(OtherActor);

	if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockLeft"));
	}

}
void ASATORI_RangeMovable::OnEndOverlapRight(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_RangeMovable* Character = Cast<ASATORI_RangeMovable>(OtherActor);

	if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockRight"));
	}

}