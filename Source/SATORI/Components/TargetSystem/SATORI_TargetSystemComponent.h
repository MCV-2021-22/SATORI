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
	float PlusDistanceToNotLoseTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float BreakLineOfSightDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	FName TargetActorsWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	TEnumAsByte<ECollisionChannel> TargetableCollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	UWidgetComponent* TargetLockedOnWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	TSubclassOf<UUserWidget> LockedOnWidgetClass;

	// The Widget Draw Size for the Widget class to use when locked on Target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	float LockedOnWidgetDrawSize = 32.0f;

	// The Socket name to attach the LockedOn Widget.
//
// You should use this to configure the Bone or Socket name the widget should be attached to, and allow
// the widget to move with target character's animation (Ex: spine_03)
//
// Set it to None to attach the Widget Component to the Root Component instead of the Mesh.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
		FName LockedOnWidgetParentSocket = FName("spine_03");

	// The Relative Location to apply on Target LockedOn Widget when attached to a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
		FVector LockedOnWidgetRelativeLocation = FVector(0.0f, 0.0f, 0.0f);

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

	FTimerHandle LineOfSightBreakTimerHandle;
	FTimerHandle SwitchingTargetTimerHandle;

	bool bIsBreakingLineOfSight = false;
	bool bIsSwitchingTarget = false;
	bool bTargetLocked = false;
	float ClosestTargetDistance = 0.0f;

	TArray<AActor*> FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax);
	AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors);

	bool LineTraceForActor(AActor* OtherActor);
	bool LineTraceForActor(AActor* OtherActor, const TArray<AActor*> ActorsToIgnore);

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

	//Switch Target
	
	bool ShouldSwitchTargetActor(const float AxisValue);
	void ResetIsSwitchingTarget();

	//Testing
	float StartRotatingStack = 0.0f;
	float AxisMultiplier = 1.0f;
	float StickyRotationThreshold = 30.0f;
	bool bDesireToSwitch = false;
	float StartRotatingThreshold = 0.85f;

	//~ Widget

	void CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};