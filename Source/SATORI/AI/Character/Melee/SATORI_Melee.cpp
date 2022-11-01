// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Melee/SATORI_Melee.h"
#include "SATORICharacter.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

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

void ASATORI_Melee::BeginPlay()
{
	Super::BeginPlay();

	TArray<UMaterialInterface*> Weapon_LeftMaterials = SwordComponentLeft->GetMaterials();
	TArray<UMaterialInterface*> Weapon_RigthMaterials = SwordComponentRight->GetMaterials();

	if (Weapon_LeftMaterials.Num() > 0)
	{
		for (int i = 0; i < Weapon_LeftMaterials.Num(); i++)
		{
			UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(Weapon_LeftMaterials[i], this);
			Left_WeaponDynamicMaterials.Add(Material);
			SwordComponentLeft->SetMaterial(i, Left_WeaponDynamicMaterials[i]);
		}
	}

	if (Weapon_RigthMaterials.Num() > 0)
	{
		for (int i = 0; i < Weapon_RigthMaterials.Num(); i++)
		{
			UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(Weapon_RigthMaterials[i], this);
			Right_WeaponDynamicMaterials.Add(Material);
			SwordComponentRight->SetMaterial(i, Right_WeaponDynamicMaterials[i]);
		}
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
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), MeleeAttack, this->GetActorTransform(), true);
			}
			else
			{ 
				USATORI_BlueprintLibrary::ApplyGameplayEffect(PlayerCharacter, HitGameplayEffect);
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), MeleeAttack, this->GetActorTransform(), true);
			}
			OverlappedComp->SetGenerateOverlapEvents(false);
		}
	}
}

void ASATORI_Melee::WeaponDissolveAfterDeath()
{
	if (Left_WeaponDynamicMaterials.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(Left_MaterialWaitHandle, [this]()
			{
				Left_WeaponDynamicMaterials[0]->SetScalarParameterValue(FName(TEXT("Appearance")), Left_Weapon_TimeCountDown);
				Left_Weapon_TimeCountDown -= LocalRate;
				if (Left_Weapon_TimeCountDown <= 0)
				{
					GetWorld()->GetTimerManager().ClearTimer(Left_MaterialWaitHandle);
				}
			}, LocalRate, true);
	}

	if (Right_WeaponDynamicMaterials.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(Right_MaterialWaitHandle, [this]()
			{
				Right_WeaponDynamicMaterials[0]->SetScalarParameterValue(FName(TEXT("Appearance")), Right_Weapon_TimeCountDown);
				Right_Weapon_TimeCountDown -= LocalRate;
				if (TimeCountDown <= 0)
				{
					GetWorld()->GetTimerManager().ClearTimer(Right_MaterialWaitHandle);
				}
			}, LocalRate, true);
	}
}