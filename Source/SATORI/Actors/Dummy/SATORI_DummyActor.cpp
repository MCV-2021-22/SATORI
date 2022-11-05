// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Dummy/SATORI_DummyActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
ASATORI_DummyActor::ASATORI_DummyActor()
{
	/*SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dummy"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);*/
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_DummyActor::OnDamageOverlapBegin);
}

// Called when the game starts or when spawned
void ASATORI_DummyActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASATORI_DummyActor::PlayImpactAnimations()
{

}

void ASATORI_DummyActor::OnDamageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ASATORI_DummyActor::OnDamageOverlapEnd(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ASATORI_DummyActor::CheckImpactReceivedByPlayer(EComboState State)
{
	switch (State)
	{
	case EComboState::LightAttack_1:
		LightAttack_1 = true;
		break;
	case EComboState::LightAttack_2:
		LightAttack_2 = true;
		break;
	case EComboState::LightAttack_3:
		LightAttack_3 = true;
		break;
	case EComboState::HeavyAttack:
		HeavyAttack = true;
		break;
	case EComboState::None:
		HeavyAttack = false;
		LightAttack_1 = false;
		LightAttack_2 = false;
		LightAttack_3 = false;
		break;
	default: break;
	}
}

void ASATORI_DummyActor::PlayAnimationWithDamage()
{
	USkeletalMeshComponent* DummyMesh = GetMesh();
	if (DummyMesh)
	{
		if (HeavyAttack || LightAttack_1 || LightAttack_2 || LightAttack_3)
		{
			DummyMesh->Stop();
			int RandomNumber = FMath::RandRange(0, 2);
			if (RandomNumber == 0)
			{
				DummyMesh->PlayAnimation(AnimToPlay_1, false);
			}
			else if (RandomNumber == 1)
			{
				DummyMesh->PlayAnimation(AnimToPlay_2, false);
			}
			else if (RandomNumber == 2)
			{
				DummyMesh->PlayAnimation(AnimToPlay_3, false);
			}
		}
	}
}

void ASATORI_DummyActor::PlayDamagedAnimation()
{
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		//AnimInstance->StopAllMontages(1.0f);
		int RandomNumber = FMath::RandRange(0, 2);
		if (RandomNumber == 0 && AnimMontageToPlay_1)
		{
			AnimInstance->Montage_Play(AnimMontageToPlay_1);
		}
		else if (RandomNumber == 1 && AnimMontageToPlay_2)
		{
			AnimInstance->Montage_Play(AnimMontageToPlay_2);
		}
		else if (RandomNumber == 2 && AnimMontageToPlay_3)
		{
			AnimInstance->Montage_Play(AnimMontageToPlay_3);
		}
	}
}