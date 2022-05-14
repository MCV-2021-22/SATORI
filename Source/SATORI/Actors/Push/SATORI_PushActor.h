
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "SATORI_PushActor.generated.h"

class USphereComponent;

UCLASS()
class SATORI_API ASATORI_PushActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SphereRadius;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float TimeToDestroy;

	ASATORI_PushActor();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent = nullptr;

	UFUNCTION()
	void OnOverlapSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	TArray<UPrimitiveComponent*> ArrayPushed;

	void OnTimerExpiredDestroy();

};
