#pragma once

//Thanks to Mickael Daniel 
//Simplified Target System based on his on https://github.com/mklabs/ue4-targetsystemplugin 

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SATORI_TargetSystemComponent.generated.h"

class UUserWidget;
class UWidgetComponent;
class APlayerController;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SATORI_API USATORI_TargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USATORI_TargetSystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float MinimumDistanceToTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	FName TargetActorsWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	TEnumAsByte<ECollisionChannel> TargetableCollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	UWidgetComponent* TargetLockedOnWidgetComponent;

	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActor();

	// Function to call to manually untarget.
	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetLockOff();

	// Function to call to manually untarget.
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

private:

	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY()
	APawn* OwnerPawn;

	UPROPERTY()
	APlayerController* OwnerPlayerController;

	UPROPERTY()
	AActor* LockedOnTargetActor;

	bool bIsBreakingLineOfSight = false;
	bool bIsSwitchingTarget = false;
	bool bTargetLocked = false;
	float ClosestTargetDistance = 0.0f;

	TArray<AActor*> FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax);
	AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors);

	bool LineTrace(FHitResult& HitResult, const AActor* OtherActor);
	bool LineTraceForActor(AActor* OtherActor);

	bool ShouldBreakLineOfSight();
	void BreakLineOfSight();

	bool IsInViewport(const AActor* TargetActor);

	float GetDistanceFromCharacter(const AActor* OtherActor);

	//~ Actor rotation

	FRotator GetControlRotationOnTarget(const AActor* OtherActor);
	void SetControlRotationOnTarget(AActor* TargetActor);
	void ControlRotation(bool ControlRotation);

	float GetAngleUsingCameraRotation(const AActor* ActorToLook);
	float GetAngleUsingCharacterRotation(const AActor* ActorToLook);

	static FRotator FindLookAtRotation(const FVector Start, const FVector Target);

	//~ Widget

	void CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};