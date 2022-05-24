//

#include "Components/TargetSystem/SATORI_TargetSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "SATORI/Character/SATORI_CharacterBase.h"

//Debug
#include "DrawDebugHelpers.h"

USATORI_TargetSystemComponent::USATORI_TargetSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USATORI_TargetSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!TargetActorsWithTag.IsValid() || !TagApliedTargeting.IsValid() || !TagApliedToEnemyTargeted.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_TargetSystemComponent: Tag is not valid ... "), *GetName());
	}

	//Check if component is on player pawn
	OwnerActor = GetOwner(); 
	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TargetSystemComponent: Cannot get owner actor ... "), *GetName());
		return;
	}

	OwnerPawn = Cast<APawn>(OwnerActor);
	if (!ensure(OwnerPawn) || !OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TargetSystemComponent: Cannot get owner pawn ... "), *GetName());
		return;
	}

	CharacterTargeting = Cast<ASATORI_CharacterBase>(OwnerActor);
	if (!CharacterTargeting)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TargetSystemComponent: Cannot get owner base character ... "), *GetName());
		return;
	}
	
	OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!OwnerPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TargetSystemComponent: Cannot get owner controller ... "), *GetName());
		return;
	}
}

void USATORI_TargetSystemComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Is target locked
	if (!bTargetLocked || !LockedOnTargetActor)
	{
		return;
	}

	//Actor Locked has targeted tag
	if (!LockedOnTargetActor->ActorHasTag(TargetActorsWithTag.GetTagName()))
	{
		TargetLockOff();
		return;
	}

	//Set rotation of player to Target Locked
	SetControlRotationOnTarget(LockedOnTargetActor);

	//Check if player is in range for continue targeting
	if (GetDistanceFromCharacter(LockedOnTargetActor) > MinimumDistanceToTarget + PlusDistanceToNotLoseTarget)
	{
		TargetLockOff();
	}

	//Check if should break target
	if (ShouldBreakLineOfSight() && !bIsBreakingLineOfSight)
	{
		if (BreakLineOfSightDelay <= 0)
		{
			TargetLockOff();
		}
		else
		{
			//Starts a timer, if when the timer ends the LineTrace fails again, will lock off
			bIsBreakingLineOfSight = true;
			GetWorld()->GetTimerManager().SetTimer(
				LineOfSightBreakTimerHandle,
				this,
				&USATORI_TargetSystemComponent::BreakLineOfSight,
				BreakLineOfSightDelay
			);
		}
	}

	//Debug
	if(IsValid(LockedOnTargetActor))
	DrawDebugLine(
	GetWorld(),
	OwnerActor->GetActorLocation(),
	LockedOnTargetActor->GetActorLocation(),
	FColor(0, 255, 0),
	false, 1.0f, 0,
	1
	);

}

void USATORI_TargetSystemComponent::TargetActor()
{
	ClosestTargetDistance = MinimumDistanceToTarget;

	if (bTargetLocked)
	{
		TargetLockOff();
	}
	else 
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorsWithTag.GetTagName(), Actors);
		LineOfSightIgnoreActors = Actors;
		LockedOnTargetActor = FindNearestTarget(Actors);
		TargetLockOn(LockedOnTargetActor);
	}
}

void USATORI_TargetSystemComponent::TargetLockOff()
{
	bTargetLocked = false;

	//Remove tags
	CharacterTargeting->RemoveGameplayTag(TagApliedTargeting);

	if (IsValid(LockedOnTargetActor))
	{
		CharacterTargeted = Cast<ASATORI_CharacterBase>(LockedOnTargetActor);
		CharacterTargeted->RemoveGameplayTag(TagApliedToEnemyTargeted);
	}

	LockedOnTargetActor = nullptr;
	
	//Ignore controller input reset
	OwnerPlayerController->ResetIgnoreLookInput();

	LineOfSightIgnoreActors.Empty();
}

void USATORI_TargetSystemComponent::TargetLockOn(AActor* TargetToLockOn)
{
	if (!IsValid(TargetToLockOn))
	{
		return;
	}

	bTargetLocked = true;

	//Add tags
	CharacterTargeted = Cast<ASATORI_CharacterBase>(TargetToLockOn);
	CharacterTargeted->AddGameplayTag(TagApliedToEnemyTargeted);
	
	CharacterTargeting->AddGameplayTag(TagApliedTargeting);
	
	//Ignore controller input
	OwnerPlayerController->SetIgnoreLookInput(true);

	LineOfSightIgnoreActors.Remove(TargetToLockOn);
}

AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors)
{
	//LineTrace for all actors with valid tag
	TArray<AActor*> ActorsHit;
	for (AActor* Actor : Actors)
	{
		bool bHit = LineTraceForActor(Actor);
		if (bHit && IsInViewport(Actor))
		{
			ActorsHit.Add(Actor);
		}
	}

	//If all fail null
	if (ActorsHit.Num() == 0)
	{
		return nullptr;
	}

	//Check the closest actor hitted with the linetrace
	float ClosestDistance = ClosestTargetDistance;
	AActor* Target = nullptr;
	for (AActor* Actor : ActorsHit)
	{
		const float Distance = GetDistanceFromCharacter(Actor);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			Target = Actor;
		}
	}

	return Target;
}

bool USATORI_TargetSystemComponent::LineTraceForActor(AActor* OtherActor)
{

	FHitResult HitResult;
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActor(OwnerActor);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		OwnerActor->GetActorLocation(),
		OtherActor->GetActorLocation(),
		TargetableCollisionChannel,
		Params
	);

	if (bHit)
	{
		if (HitResult.GetActor() == OtherActor)
		{
			return true;
		}
	}

	return false;
}

bool USATORI_TargetSystemComponent::LineTraceForActor(AActor* OtherActor, const TArray<AActor*> ActorsToIgnore)
{

	FHitResult HitResult;
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActor(OwnerActor);
	Params.AddIgnoredActors(ActorsToIgnore);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		OwnerActor->GetActorLocation(),
		OtherActor->GetActorLocation(),
		TargetableCollisionChannel,
		Params
	);

	if (bHit)
	{
		if (HitResult.GetActor() == OtherActor)
		{
			return true;
		}
	}

	return false;
}

bool USATORI_TargetSystemComponent::ShouldBreakLineOfSight()
{
	if (!LockedOnTargetActor)
	{
		return true;
	}

	//Check if target actor is on sight
	if (LineTraceForActor(LockedOnTargetActor, LineOfSightIgnoreActors)) //Should not break line of sight if pawn
	{
		return false;
	}
	return true;
}

void USATORI_TargetSystemComponent::BreakLineOfSight()
{
	bIsBreakingLineOfSight = false;
	if (ShouldBreakLineOfSight())
	{
		TargetLockOff();
	}
}

bool USATORI_TargetSystemComponent::IsInViewport(const AActor* TargetActor)
{
	FVector2D ScreenLocation;
	OwnerPlayerController->ProjectWorldLocationToScreen(TargetActor->GetActorLocation(), ScreenLocation);

	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	return ScreenLocation.X > 0 && ScreenLocation.Y > 0 && ScreenLocation.X < ViewportSize.X&& ScreenLocation.Y < ViewportSize.Y;
}

void USATORI_TargetSystemComponent::SetControlRotationOnTarget(AActor* TargetActor)
{
	FRotator ControlRotation = GetControlRotationOnTarget(TargetActor);
	OwnerPlayerController->SetControlRotation(ControlRotation);
}

FRotator USATORI_TargetSystemComponent::GetControlRotationOnTarget(const AActor* OtherActor)
{
	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();

	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector OtherActorLocation = OtherActor->GetActorLocation();

	const FRotator LookRotation = FRotationMatrix::MakeFromX(OtherActorLocation - CharacterLocation).Rotator();
	float Pitch = LookRotation.Pitch;
	FRotator TargetRotation;

	TargetRotation = FRotator(ControlRotation.Pitch, LookRotation.Yaw, ControlRotation.Roll);

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 9.0f);
}

float USATORI_TargetSystemComponent::GetAngleUsingCameraRotation(const AActor* ActorToLook)
{
	UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>();
	if (!CameraComponent)
	{
		return GetAngleUsingCharacterRotation(ActorToLook);
	}

	const FRotator CameraWorldRotation = CameraComponent->GetComponentRotation();
	const FRotator LookAtRotation = FindLookAtRotation(CameraComponent->GetComponentLocation(), ActorToLook->GetActorLocation());

	float YawAngle = CameraWorldRotation.Yaw - LookAtRotation.Yaw;
	if (YawAngle < 0)
	{
		YawAngle = YawAngle + 360;
	}

	return YawAngle;
}

float USATORI_TargetSystemComponent::GetAngleUsingCharacterRotation(const AActor* ActorToLook)
{
	const FRotator CharacterRotation = OwnerActor->GetActorRotation();
	const FRotator LookAtRotation = FindLookAtRotation(OwnerActor->GetActorLocation(), ActorToLook->GetActorLocation());

	float YawAngle = CharacterRotation.Yaw - LookAtRotation.Yaw;
	if (YawAngle < 0)
	{
		YawAngle = YawAngle + 360;
	}

	return YawAngle;
}

FRotator USATORI_TargetSystemComponent::FindLookAtRotation(const FVector Start, const FVector Target)
{
	return FRotationMatrix::MakeFromX(Target - Start).Rotator();
}

void USATORI_TargetSystemComponent::TargetActorWithAxisInput(const float AxisValue)
{
	if (!bTargetLocked || !LockedOnTargetActor)
	{
		return;
	}

	// If we're not allowed to switch target, do nothing
	if (!ShouldSwitchTargetActor(AxisValue))
	{
		return;
	}

	// If we're switching target, do nothing for a set amount of time
	if (bIsSwitchingTarget)
	{
		return;
	}

	AActor* CurrentTarget = LockedOnTargetActor;

	// Depending on Axis Value negative / positive, set Direction to Look for (negative: left, positive: right)
	const float RangeMin = AxisValue < 0 ? 0 : 180;
	const float RangeMax = AxisValue < 0 ? 180 : 360;

	ClosestTargetDistance = MinimumDistanceToTarget;

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorsWithTag.GetTagName(), Actors);

	TArray<AActor*> ActorsToLook;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CurrentTarget);

	for (AActor* Actor : Actors)
	{
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
		{
			ActorsToLook.Add(Actor);
		}
	}

	// Find Targets in Range (left or right, based on Character and CurrentTarget)
	TArray<AActor*> TargetsInRange = FindTargetsInRange(ActorsToLook, RangeMin, RangeMax);

	// For each of these targets in range, get the closest one to current target
	AActor* ActorToTarget = nullptr;
	for (AActor* Actor : TargetsInRange)
	{
		// and filter out any character too distant from minimum distance to enable
		const float Distance = GetDistanceFromCharacter(Actor);
		if (Distance < MinimumDistanceToTarget + PlusDistanceToNotLoseTarget)
		{
			const float RelativeActorsDistance = CurrentTarget->GetDistanceTo(Actor);
			if (RelativeActorsDistance < ClosestTargetDistance)
			{
				ClosestTargetDistance = RelativeActorsDistance;
				ActorToTarget = Actor;
			}
		}
	}

	if (ActorToTarget)
	{
		if (SwitchingTargetTimerHandle.IsValid())
		{
			SwitchingTargetTimerHandle.Invalidate();
		}

		TargetLockOff();
		LockedOnTargetActor = ActorToTarget;
		TargetLockOn(ActorToTarget);

		GetWorld()->GetTimerManager().SetTimer(
			SwitchingTargetTimerHandle,
			this,
			&USATORI_TargetSystemComponent::ResetIsSwitchingTarget,
			// Less sticky if still switching
			bIsSwitchingTarget ? 0.25f : 0.5f
		);

		bIsSwitchingTarget = true;
	}
}

bool USATORI_TargetSystemComponent::ShouldSwitchTargetActor(const float AxisValue)
{
	return FMath::Abs(AxisValue) > StartRotatingThreshold;
}

TArray<AActor*> USATORI_TargetSystemComponent::FindTargetsInRange(TArray<AActor*> ActorsToLook, const float RangeMin, const float RangeMax)
{
	TArray<AActor*> ActorsInRange;

	for (AActor* Actor : ActorsToLook)
	{
		const float Angle = GetAngleUsingCameraRotation(Actor);
		if (Angle > RangeMin && Angle < RangeMax)
		{
			ActorsInRange.Add(Actor);
		}
	}

	return ActorsInRange;
}

void USATORI_TargetSystemComponent::ResetIsSwitchingTarget()
{
	bIsSwitchingTarget = false;
	bDesireToSwitch = false;
}

bool USATORI_TargetSystemComponent::GetTargetLockedStatus()
{
	return bTargetLocked;
}

AActor* USATORI_TargetSystemComponent::GetLockedOnTargetActor()
{
	return LockedOnTargetActor;
}

bool USATORI_TargetSystemComponent::IsLocked()
{
	return bTargetLocked && LockedOnTargetActor;
}

float USATORI_TargetSystemComponent::GetDistanceFromCharacter(const AActor* OtherActor)
{
	return OwnerActor->GetDistanceTo(OtherActor);
}