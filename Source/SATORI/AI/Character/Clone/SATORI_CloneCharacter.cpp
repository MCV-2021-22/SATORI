//

#include "AI/Character/Clone/SATORI_CloneCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_CloneCharacter::ASATORI_CloneCharacter() 
{
	// Weapon Component
	SwordComponentClone = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordClone"));
	AttackingCollisionClone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sword CollisionClone"));
	if (SwordComponentClone)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponentClone->AttachToComponent(GetMesh(), AttachmentRules, "BoSocket");
		// Sphere Collision
		AttackingCollisionClone->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollisionClone->SetCollisionProfileName("Pawn");
		AttackingCollisionClone->SetGenerateOverlapEvents(false);
		AttackingCollisionClone->AttachToComponent(SwordComponentClone, AttachmentRules);

		AttackingCollisionClone->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_CloneCharacter::OnWeaponOverlapBegin);
	}
}

void ASATORI_CloneCharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ASATORI_AICharacter* EnemyCharacter = Cast<ASATORI_AICharacter>(OtherActor);
		if (EnemyCharacter)
		{
			UAbilitySystemComponent* EnemyAbilitySystem = EnemyCharacter->GetAbilitySystemComponent();
			float Damage_Values = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(EnemyCharacter, WeaponDamage, this, DamageEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(EnemyCharacter, BlockCountGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(EnemyCharacter, StunGameplayEffect);
			if (!bMultipleHit)
			{
				AttackingCollision->SetGenerateOverlapEvents(false);
			}
			EnemyCharacter->CheckDamage();
		}
	}
}

void ASATORI_CloneCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Character.Clone");
}