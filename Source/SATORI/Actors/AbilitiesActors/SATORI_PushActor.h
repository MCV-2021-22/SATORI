//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SATORI_PushActor.generated.h"

class USphereComponent;

UCLASS()
class SATORI_API ASATORI_PushActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASATORI_PushActor();

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float PushForce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float TimeToDestroy;

	UFUNCTION(BlueprintCallable, Category = "Push")
	void OnOverlapSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  PlayerTag;

protected:

	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

private:

	TArray<UPrimitiveComponent*> ArrayPushed;

	FTimerHandle TimerHandleDestroy;

	void DestroyMyself();

};
