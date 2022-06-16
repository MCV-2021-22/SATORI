//

#include "Actors/MapLimit/SATORI_MapLimit.h"
#include "Components/BoxComponent.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "SATORICharacter.h"

ASATORI_MapLimit::ASATORI_MapLimit()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(FName("Trigger"));

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_MapLimit::OnComponentBeginOverlap);

	Tags.Add("LimitMap");
	

}

void ASATORI_MapLimit::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);

	if(!Player)
	{
		return;
	}

	

}


