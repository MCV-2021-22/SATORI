//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_PushActor.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UStaticMeshComponent;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float PushForce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float TraceDistanceToFloor = 250.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float HeightChange = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float MaxHeight = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	float MinHeight = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
	bool bDrawDebug = false;

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
	FGameplayTag  PushedTag;

protected:

	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

private:

	TArray<AActor*> ArrayPushed;

	FTimerHandle TimerHandleDestroy;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void DestroyMyself();

};
