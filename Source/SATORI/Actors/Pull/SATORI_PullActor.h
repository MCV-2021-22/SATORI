
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SATORI_PullActor.generated.h"

class USphereComponent;

UCLASS()
class SATORI_API ASATORI_PullActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SphereRadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SpeedForward;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SpeedPulling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float TimeToDestroy;

	ASATORI_PullActor();

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

	void OnTimerExpiredDestroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FVector Start;

	UPrimitiveComponent* Pulling = nullptr;

};
