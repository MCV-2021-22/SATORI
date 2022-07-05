//

#include "AI/Character/Clone/SATORI_CloneCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

ASATORI_CloneCharacter::ASATORI_CloneCharacter() 
{

	LuringSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	LuringSphereComponent->SetCollisionProfileName(FName(TEXT("PlayerAbility")));
	LuringSphereComponent->SetupAttachment(RootComponent);
	LuringSphereComponent->SetGenerateOverlapEvents(true);
	LuringSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_CloneCharacter::OnOverlapLuringSphere);

	//Debug
	LuringSphereComponent->bHiddenInGame = false;

	if (SwordComponent)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponent->AttachToComponent(GetMesh(), AttachmentRules, "BoSocket");
		// Sphere Collision
		AttackingCollision->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollision->SetCollisionProfileName("Pawn");
		AttackingCollision->SetGenerateOverlapEvents(false);
		AttackingCollision->AttachTo(SwordComponent);
	}

}

//Collision for luring
void ASATORI_CloneCharacter::OnOverlapLuringSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ASATORI_CharacterBase* Character = Cast<ASATORI_CharacterBase>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character->HasMatchingGameplayTag(EnemyTag))
	{
		Character->AddGameplayTag(TagGrantedWhenLured);
		ArrayLured.AddUnique(OtherActor);
	}
}

void ASATORI_CloneCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Character.Clone");
}