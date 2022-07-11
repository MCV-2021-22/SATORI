#pragma once

//Thanks to Mickael Daniel 
//Modified Target System based on his on https://github.com/mklabs/ue4-targetsystemplugin 

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "SATORI_TargetSystemComponent.generated.h"

class APlayerController;
class UWidgetComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SATORI_API USATORI_TargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USATORI_TargetSystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0.0"), Category = "Target System")
	float MinimumDistanceToTarget = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0.0"), Category = "Target System")
	float AddedDistanceToNotLoseTarget = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0.0"), Category = "Target System")
	float BreakLineOfSightDelay = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	TEnumAsByte<ECollisionChannel> TargetableCollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	TSubclassOf<UUserWidget> LockedOnWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	FName LockedOnWidgetParentSocket = FName("Bip001-Head");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	FVector LockedOnWidgetRelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Widget")
	float LockedOnWidgetDrawSize = 32.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System")
	float StartRotatingThreshold = 0.85f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Sticky Feeling on Target Switch")
	bool bEnableStickyTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Sticky Feeling on Target Switch")
	float AxisMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Sticky Feeling on Target Switch")
	float StickyRotationThreshold = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchDistanceCoefficient = -0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchDistanceOffset = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchMin = -50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Pitch Offset")
	float PitchMax = -20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target System|Debug")
	bool bShowDebugLine = false;

	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActor();

	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetActorWithAxisInput(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetLockOff();

	UFUNCTION(BlueprintCallable, Category = "Target System")
	void TargetLockOn(AActor* TargetToLockOn);

	UFUNCTION(BlueprintCallable, Category = "Target System")
	bool GetTargetLockedStatus() const;

	UFUNCTION(BlueprintCallable, Category = "Target System")
	AActor* GetLockedOnTargetActor() const;

	UFUNCTION(BlueprintCallable, Category = "Target System")
	bool IsLocked() const;

	UFUNCTION(BlueprintCallable, Category = "Target System")
	bool IsInViewport(const AActor* TargetActor) const;

private:

	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY()
	APawn* OwnerPawn;

	UPROPERTY()
	ASATORI_PlayerController* OwnerPlayerController;

	UPROPERTY()
	AActor* LockedOnTargetActor;

	UPROPERTY()
	UWidgetComponent* TargetLockedOnWidgetComponent;

	void CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor);

	UPROPERTY()
	TArray<AActor*> LineOfSightIgnoreActors;

	FTimerHandle LineOfSightBreakTimerHandle;
	FTimerHandle SwitchingTargetTimerHandle;
	

	bool bIsBreakingLineOfSight = false;
	bool bIsSwitchingTarget = false;
	bool bTargetLocked = false;

	float ClosestTargetDistance = 0.0f;
	float StartRotatingStack = 0.0f;

	void ControlRotation(const bool ShouldControlRotation);

	static bool TargetIsTargetable(const AActor* Actor);

	TArray<AActor*> FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax) const;
	AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors) const;

	bool LineTraceForActor(AActor* OtherActor) const;
	bool LineTraceForActor(AActor* OtherActor, const TArray<AActor*> ActorsToIgnore) const;

	float GetAngleUsingCameraRotation(const AActor* ActorToLook) const;
	float GetAngleUsingCharacterRotation(const AActor* ActorToLook) const;

	float GetDistanceFromCharacter(const AActor* OtherActor) const;

	//~ Break linesight

	bool ShouldBreakLineOfSight() const;
	void BreakLineOfSight();

	//~ Actor rotation

	FRotator GetControlRotationOnTarget(const AActor* OtherActor) const;
	void SetControlRotationOnTarget(AActor* TargetActor);

	static FRotator FindLookAtRotation(const FVector Start, const FVector Target);

	//Switch Target
	
	bool bDesireToSwitch = false;
	
	bool ShouldSwitchTargetActor(const float AxisValue);
	void ResetIsSwitchingTarget();

protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};