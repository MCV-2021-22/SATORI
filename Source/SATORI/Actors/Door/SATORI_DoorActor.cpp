//

#include "Actors/Door/SATORI_DoorActor.h"
#include "Components/BoxComponent.h"

ASATORI_DoorActor::ASATORI_DoorActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(FName("Trigger"));

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_DoorActor::OnComponentBeginOverlap);

}

void ASATORI_DoorActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] ASATORI_DoorActor: Overlap with: %s"), *GetName(), *OtherActor->GetName());
	


}