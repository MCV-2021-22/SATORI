//

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_CloneCharacter.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_CloneCharacter : public ASATORI_AICharacter
{
	GENERATED_BODY()
	
public:

	ASATORI_CloneCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Clone")
	USphereComponent* LuringSphereComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Clone")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Clone")
	FGameplayTag TagGrantedWhenLured;

	UFUNCTION(BlueprintCallable, Category = "Clone")
		void OnOverlapLuringSphere(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clone|Tags")
	FGameplayTag  TargetActorWithTag;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clone|Tags")
	//FGameplayTag  EnemyTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clone|Tags")
	FGameplayTag  PlayerTag;

private:

	AActor* Target;

	TArray<AActor*> ArrayLured;

};
