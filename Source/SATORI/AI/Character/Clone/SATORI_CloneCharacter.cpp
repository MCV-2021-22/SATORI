//

#include "AI/Character/Clone/SATORI_CloneCharacter.h"
#include "Components/SphereComponent.h"

ASATORI_CloneCharacter::ASATORI_CloneCharacter() 
{

	LuringSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	LuringSphereComponent->SetCollisionProfileName(FName(TEXT("IgnoreSelfOverlapsAll")));
	LuringSphereComponent->SetupAttachment(RootComponent);
	LuringSphereComponent->SetGenerateOverlapEvents(true);
	LuringSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASATORI_CloneCharacter::OnOverlapLuringSphere);

	//Debug
	LuringSphereComponent->bHiddenInGame = false;
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