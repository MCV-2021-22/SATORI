#pragma once

//Thanks to Mickael Daniel 
//Simplified Target System based on his on https://github.com/mklabs/ue4-targetsystemplugin 

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SATORI_TargetSystemComponent.generated.h"

class APlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SATORI_API USATORI_TargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USATORI_TargetSystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "100.0"), Category = "Target System")
	float MinimumDistanceToTarget = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0.0"), Category = "Target System")
	float PlusDistanceToNotLoseTarget = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "1.0"), Category = "Target System")
	float BreakLineOfSightDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	FGameplayTag TargetActorsWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	FGameplayTag TagApliedTargeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	FGameplayTag TagApliedToEnemyTargeted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	TEnumAsByte<ECollisionChannel> TargetableCollisionChannel =  ECollisionChannel::ECC_Pawn;

	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActor();

	// Function to call to manually untarget.
	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetLockOff();

	// Function to call to manually target.
	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetLockOn(AActor* TargetToLockOn);

	// Function to get TargetLocked private variable status
	UFUNCTION(BlueprintCallable, Category = "Target System")
	bool GetTargetLockedStatus();

	// Returns the reference to currently targeted Actor if any
	UFUNCTION(BlueprintCallable, Category = "Target System")
	AActor* GetLockedOnTargetActor();

	// Returns true / false whether the system is targeting an actor
	UFUNCTION(BlueprintCallable, Category = "Target System")
	bool IsLocked();

	//Function to call for switching Targets based on Axis input
	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActorWithAxisInput(float AxisValue);

private:

	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY()
	APawn* OwnerPawn;

	UPROPERTY()
	APlayerController* OwnerPlayerController;

	UPROPERTY()
	AActor* LockedOnTargetActor;

	UPROPERTY()
	TArray<AActor*> LineOfSightIgnoreActors;

	FTimerHandle LineOfSightBreakTimerHandle;
	FTimerHandle SwitchingTargetTimerHandle;

	bool bIsBreakingLineOfSight = false;
	bool bIsSwitchingTarget = false;
	bool bTargetLocked = false;
	float ClosestTargetDistance = 0.0f;

	//Tags
	ASATORI_CharacterBase* CharacterTargeted;
	ASATORI_CharacterBase* CharacterTargeting;

	TArray<AActor*> FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax);
	AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors);

	bool LineTraceForActor(AActor* OtherActor);
	bool LineTraceForActor(AActor* OtherActor, const TArray<AActor*> ActorsToIgnore);

	bool IsInViewport(const AActor* TargetActor);

	float GetDistanceFromCharacter(const AActor* OtherActor);

	//~ Break linesight

	bool ShouldBreakLineOfSight();
	void BreakLineOfSight();

	//~ Actor rotation

	FRotator GetControlRotationOnTarget(const AActor* OtherActor);
	void SetControlRotationOnTarget(AActor* TargetActor);

	float GetAngleUsingCameraRotation(const AActor* ActorToLook);
	float GetAngleUsingCharacterRotation(const AActor* ActorToLook);

	static FRotator FindLookAtRotation(const FVector Start, const FVector Target);

	//Switch Target
	
	bool ShouldSwitchTargetActor(const float AxisValue);
	void ResetIsSwitchingTarget();

	bool bDesireToSwitch = false;
	float StartRotatingThreshold = 0.85f;

protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};