// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Melee/SATORI_Melee.h"
#include "SATORICharacter.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "Components/CapsuleComponent.h"

ASATORI_Melee::ASATORI_Melee()
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Melee/BT_MELEE.BT_MELEE")));

	// Weapon Component Right
	SwordComponentRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordRight"));
	AttackingCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollisionRight"));
	if (SwordComponentRight)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponentRight->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket2");
		// Sphere Collision
		AttackingCollision->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollision->SetCollisionProfileName("Pawn");
		AttackingCollision->SetGenerateOverlapEvents(false);
		AttackingCollision->AttachTo(SwordComponentRight);

		AttackingCollision->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_Melee::OnWeaponOverlapBegin);
	}
	// Weapon Component Left
	SwordComponentLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordLeft"));
	AttackingCollisionLeft = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollisionLeft"));
	if (SwordComponentLeft)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponentLeft->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
		// Sphere Collision
		AttackingCollisionLeft->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollisionLeft->SetCollisionProfileName("Pawn");
		AttackingCollisionLeft->SetGenerateOverlapEvents(false);
		AttackingCollisionLeft->AttachTo(SwordComponentLeft);

		AttackingCollisionLeft->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_Melee::OnWeaponOverlapBegin);
	}
}

void ASATORI_Melee::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(OtherActor);
		if (PlayerCharacter)
		{
			UAbilitySystemComponent* PlayerAbilitySystem = PlayerCharacter->GetAbilitySystemComponent();
			USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(PlayerCharacter, WeaponDamage, this, DamageEffect);
			if (this->HasMatchingGameplayTag(HeavyHitTag))
			{
				USATORI_BlueprintLibrary::ApplyGameplayEffect(PlayerCharacter, HeavyHitGameplayEffect);
			}
			else
			{ 
				USATORI_BlueprintLibrary::ApplyGameplayEffect(PlayerCharacter, HitGameplayEffect);
			}
			OverlappedComp->SetGenerateOverlapEvents(false);
		}
	}
}