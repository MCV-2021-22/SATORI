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

	bool Pushing = false;

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Push")
	float TimeToFinish;

	//Movement
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float Speed = 6000.0f;

	//Final launch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	float LaunchForce = 800.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	float ZLaunching = 80.0f;

	//Debug movement
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	float HeightCorrection = 100.0f;

	//Stay Grounded
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float TraceDistanceToGround = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float MinDistanceToGround = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float MaxDistanceToGround = 50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	float HeightChange = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Grounded")
	bool bDrawDebug = false;

	UFUNCTION(BlueprintCallable, Category = "Push")
	void OnOverlapCollisionSphere(
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

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void DestroyMyself();
	void FinalActions(AActor* Actor);
	void StayGrounded(float DeltaTime);
	void DamageEnemy(AActor* Actor);

};
