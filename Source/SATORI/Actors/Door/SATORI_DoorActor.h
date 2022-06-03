//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SATORI_DoorActor.generated.h"

class UBoxComponent;

UCLASS()
class SATORI_API ASATORI_DoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_DoorActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxComponent = nullptr;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
