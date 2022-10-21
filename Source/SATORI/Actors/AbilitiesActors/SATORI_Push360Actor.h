//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_Push360Actor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class SATORI_API ASATORI_Push360Actor : public AActor
{
	GENERATED_BODY()
	
public:	

	ASATORI_Push360Actor();

	UPROPERTY(EditDefaultsOnly, Category = "Push")
	USphereComponent* CollisionSphereComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Push")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystemComponent* PushParticle = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Push")
	float Damage;

	UPROPERTY(BlueprintReadOnly, Meta = (ExposeOnSpawn = true), Category = "Push")
	float TimeToFinish;

	//Final launch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	float LaunchForce = 800.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push")
	float ZLaunching = 80.0f;

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
	FGameplayTag  LaunchTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  AbilityTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  StunnedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Push|Tags")
	FGameplayTag  SpecialTag;

protected:

	virtual void BeginPlay() override;

private:

	void StopAction(ASATORI_AICharacter* Character);
	void LaunchEnemy(AActor* Actor, ASATORI_AICharacter* Character);
	void RotateEnemy(AActor* Actor);
	void DamageEnemy(AActor* Actor, ASATORI_AICharacter* Character);
	void DestroyMyself();

};
