//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_PickableAbilityComponent.h"
#include "GameplayEffect.h"
#include "SATORI_AbilityPickableActor.generated.h"

class UBoxComponent;
class UGameplayEffect;

UCLASS()
class SATORI_API ASATORI_AbilityPickableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_AbilityPickableActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USATORI_PickableAbilityComponent* AbilityComponent = nullptr;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
