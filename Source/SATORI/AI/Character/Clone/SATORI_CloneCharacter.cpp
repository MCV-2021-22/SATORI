//

#include "AI/Character/Clone/SATORI_CloneCharacter.h"

#include "AI/Character/Melee/SATORI_Melee.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"

ASATORI_CloneCharacter::ASATORI_CloneCharacter() 
{

	//If collides will lure enemies
	LuringSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("LuringRangeSphere"));
	LuringSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	LuringSphereComponent->SetupAttachment(RootComponent);
	LuringSphereComponent->SetGenerateOverlapEvents(true);
	LuringSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_CloneCharacter::OnOverlapCollisionSphereLuring);

	// Weapon Component
	SwordComponentClone = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordClone"));
	AttackingCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sword CollisionClone"));
	if (SwordComponentClone)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponentClone->AttachToComponent(GetMesh(), AttachmentRules, "BoSocket");
		// Sphere Collision
		AttackingCollision->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollision->SetCollisionProfileName("Pawn");
		AttackingCollision->SetGenerateOverlapEvents(false);
		AttackingCollision->AttachToComponent(SwordComponentClone, AttachmentRules);

		AttackingCollision->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_CloneCharacter::OnWeaponOverlapBegin);
	}
}

//Collision for luring
void ASATORI_CloneCharacter::OnOverlapCollisionSphereLuring(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Clone possible collisions : 

	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag) && !Character->HasMatchingGameplayTag(LuredTag))
	{
		Character->AddGameplayTag(LuredTag);
		ArrayLured.AddUnique(OtherActor);
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
			USATORI_BlueprintLibrary::ApplyGameplayEffect(EnemyCharacter, StunGameplayEffect);
			if (!bMultipleHit)
			{
				AttackingCollision->SetGenerateOverlapEvents(false);
			}

			ASATORI_Melee* Melee = Cast<ASATORI_Melee>(EnemyCharacter);
			if(!Melee) USATORI_BlueprintLibrary::ApplyGameplayEffect(EnemyCharacter, DamageEffect);
			EnemyCharacter->CheckDamage(WeaponDamage);
		}
	}
}

void ASATORI_CloneCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Character.Clone");
}