//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "GameplayEffect.h"
#include "SATORI_MapLimit.generated.h"

class UBoxComponent;
class UGameplayEffect;

UCLASS()
class SATORI_API ASATORI_MapLimit : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_MapLimit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxComponent = nullptr;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
