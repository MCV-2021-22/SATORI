// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Melee/SATORI_Melee.h"

#include "Components/CapsuleComponent.h"

ASATORI_Melee::ASATORI_Melee()
{
	PrimaryActorTick.bCanEverTick = true;

	bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Melee/BT_MELEE.BT_MELEE")));

	SwordComponent2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword2"));
	AttackingCollision2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sword Collision2"));
	if (SwordComponent2)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponent2->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
		// Sphere Collision
		AttackingCollision2->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollision2->SetCollisionProfileName("Pawn");
		AttackingCollision2->SetGenerateOverlapEvents(false);
		AttackingCollision2->AttachTo(SwordComponent2);
	}
}
