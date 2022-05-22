//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "SATORI_PullActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SATORI_API ASATORI_PullActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASATORI_PullActor();

	UPROPERTY(EditDefaultsOnly, Category = "Pull")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pull")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pull")
	USphereComponent* SeekingSphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Pull")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	float SpeedForward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	float SpeedPulling;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Pull")
	float TimeToDestroy;
	
	UFUNCTION(BlueprintCallable, Category = "Pull")
	void OnOverlapCollisionSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Pull")
	void OnOverlapSeekingSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pull|Tags")
	FName TargetActorWithTag = FName(TEXT("State.Targeted"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pull|Tags")
	FName EnemyTag = FName(TEXT("Enemy"));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pull|Tags")
	FName PlayerTag = FName(TEXT("Player"));

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	AActor* Target;

	FTimerHandle TimerHandleDestroy;

	FVector Start;

	UPrimitiveComponent* Pulling = nullptr;

	void DestroySelf();

};
