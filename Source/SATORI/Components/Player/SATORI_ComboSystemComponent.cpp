// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "SATORICharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
USATORI_ComboSystemComponent::USATORI_ComboSystemComponent()
{
	//isLightAttack = true;
	//isHeavyAttack = true;
	// Weapon Component
	bWantsInitializeComponent = true;
}

void USATORI_ComboSystemComponent::InitializeComponent()
{
	//SwordComponent = NewObject<USkeletalMeshComponent>(GetOwner(), TEXT("Sword"));
	//AttackingCollision = NewObject<USphereComponent>(GetOwner(), TEXT("Collision"));
	//
	///*SwordComponent->SkeletalMesh =  */

	//ASATORICharacter* Character = Cast<ASATORICharacter>(GetOwner());
	//if (SwordComponent)
	//{
	//	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	//	SwordComponent->SetupAttachment(Character->GetMesh(), "Sword_1");
	//	// Sphere Collision
	//	AttackingCollision->InitSphereRadius(40.0f);
	//	AttackingCollision->SetCollisionProfileName("Pawn");
	//	AttackingCollision->SetGenerateOverlapEvents(false);
	//	AttackingCollision->AttachTo(SwordComponent);
	//}

	//SwordComponent->CreationMethod = EComponentCreationMethod::Instance;
	//AttackingCollision->CreationMethod = EComponentCreationMethod::Instance;

	//SwordComponent->RegisterComponent();
	//AttackingCollision->RegisterComponent();

}

// Called when the game starts
void USATORI_ComboSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USATORI_ComboSystemComponent::LigthAttack()
{
	if (isLightAttack)
	{
		isHeavyAttack = false;
		//isLightAttack = false;
		if (lightAttackCounter >= LightAttackMontageToPlay.Num() - 1)
		{
			lightAttackCounter = 0;
		}
		else
		{
			lightAttackCounter ++;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("lightAttackCounter : %d"), lightAttackCounter);
}

void USATORI_ComboSystemComponent::HeavyAttack()
{
	/*if (isHeavyAttack)
	{
		isLightAttack = false;
	}*/
	
}

void USATORI_ComboSystemComponent::ResetAllAttribute()
{
	/*isLightAttack = true;
	isHeavyAttack = true;*/
	lightAttackCounter = 0;
	HeavyAttackCounter = 0;
}

bool USATORI_ComboSystemComponent::CanComboAttack()
{
	if (isLightAttack || isHeavyAttack)
		return true;

	return false;
}