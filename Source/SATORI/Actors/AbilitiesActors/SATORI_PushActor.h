//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_PushActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class SATORI_API ASATORI_PushActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASATORI_PushActor();

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	UBoxComponent* CollisionBoxComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Push")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	// Particles
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particle")
	UNiagaraSystem* Push_Particle = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Push")
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
	void OnOverlapCollisionBox(
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  LaunchTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  AbilityTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  StunnedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  SpecialTag;

protected:

	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

private:

	AActor* PushedActor;
	ASATORI_AICharacter* PushedCharacter;

	bool Pushing = false;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	void StopAction(ASATORI_AICharacter* Character);
	void LaunchEnemy(AActor* Actor, ASATORI_AICharacter* Character);
	void RotateEnemy(AActor* Actor);
	void DamageEnemy(AActor* Actor, ASATORI_AICharacter* Character);
	void StayGrounded(float DeltaTime);
	void DestroyMyself();

};
