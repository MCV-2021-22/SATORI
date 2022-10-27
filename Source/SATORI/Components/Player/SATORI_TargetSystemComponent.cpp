//

#include "SATORI_TargetSystemComponent.h"
#include "Interfaces/SATORI_TargetSystemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "SATORIGameMode.h"
#include "GameplayFramework/SATORI_GameInstance.h"
//Debug
#include "DrawDebugHelpers.h"


USATORI_TargetSystemComponent::USATORI_TargetSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USATORI_TargetSystemComponent::BeginPlay()
{
	Super::BeginPlay();

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
	
	OwnerPlayerController = Cast<ASATORI_PlayerController>(OwnerPawn->GetController());
	if (!OwnerPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TargetSystemComponent: Cannot get owner controller ... "), *GetName());
		return;
	}

	if (!LockedOnWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_TargetSystemComponent: Cannot get LockedOnWidgetClass, ensure it is a valid reference ..."), *GetName());
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

	if (ASATORI_AICharacter* IsDead = Cast<ASATORI_AICharacter>(LockedOnTargetActor))
	{
		if(IsDead->IsPendingKill())
		{
			TargetLockOff();
			return;
		}
		if (IsDead->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
		{
			TargetLockOff();
			return;
		}
	}
	
	//Actor Locked has targeted tag
	if (!TargetIsTargetable(LockedOnTargetActor))
	{
		TargetLockOff();
		return;
	}

	//Set rotation of player to Target Locked
	SetControlRotationOnTarget(LockedOnTargetActor);

	//Check if player is in range for continue targeting
	if (GetDistanceFromCharacter(LockedOnTargetActor) > MinimumDistanceToTarget + AddedDistanceToNotLoseTarget)
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
			GetWorld()->GetTimerManager().SetTimer(LineOfSightBreakTimerHandle, this, &USATORI_TargetSystemComponent::BreakLineOfSight, BreakLineOfSightDelay);
		}
	}

	//Debug
	if (bShowDebugLine)
	{
		DrawDebugLine(GetWorld(), OwnerActor->GetActorLocation(), LockedOnTargetActor->GetActorLocation(), FColor(0, 0, 255), false, 1.0f, 0, 1);
	}

}

bool USATORI_TargetSystemComponent::TargetIsTargetable(const AActor* Actor)
{
	const bool bIsImplemented = Actor->GetClass()->ImplementsInterface(USATORI_TargetSystemInterface::StaticClass());
	if (bIsImplemented)
	{
		return ISATORI_TargetSystemInterface::Execute_IsTargetable(Actor);
	}

	return true;
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
		TArray < AActor* >& AllEnemies = GetWorld()->GetAuthGameMode<ASATORIGameMode>()->GetEnemyActorsRef();
		LockedOnTargetActor = FindNearestTarget(AllEnemies);
		TargetLockOn(LockedOnTargetActor);
	}
}

void USATORI_TargetSystemComponent::TargetLockOff()
{
	bTargetLocked = false;
	LockedOnTargetActor = nullptr;

	TargetLockedOnWidgetComponent->DestroyComponent();

	LineOfSightIgnoreActors.Empty();

	//Ignore controller input reset
	OwnerPlayerController->ResetIgnoreLookInput();
}

void USATORI_TargetSystemComponent::TargetLockOn(AActor* TargetToLockOn)
{
	if (!IsValid(TargetToLockOn))
	{
		return;
	}

	bTargetLocked = true;

	CreateAndAttachTargetLockedOnWidgetComponent(TargetToLockOn);

	//It will not break line of sight with others enemies
	LineOfSightIgnoreActors = GetWorld()->GetAuthGameMode<ASATORIGameMode>()->GetEnemyActorsRef();
	LineOfSightIgnoreActors.Remove(LockedOnTargetActor);

	//Ignore controller input
	OwnerPlayerController->SetIgnoreLookInput(true);
}

AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors) const
{
	//LineTrace for all Targeteable Actors
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

bool USATORI_TargetSystemComponent::LineTraceForActor(AActor* OtherActor) const
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

bool USATORI_TargetSystemComponent::LineTraceForActor(AActor* OtherActor, const TArray<AActor*> ActorsToIgnore) const
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

bool USATORI_TargetSystemComponent::ShouldBreakLineOfSight() const
{
	if (!LockedOnTargetActor)
	{
		return true;
	}

	//Check if target actor is on sight
	if (LineTraceForActor(LockedOnTargetActor, LineOfSightIgnoreActors)) //Should not break line of sight if other enemies are in front
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

bool USATORI_TargetSystemComponent::IsInViewport(const AActor* TargetActor) const
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

FRotator USATORI_TargetSystemComponent::GetControlRotationOnTarget(const AActor* OtherActor) const
{
	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();

	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector OtherActorLocation = OtherActor->GetActorLocation();

	const FRotator LookRotation = FRotationMatrix::MakeFromX(OtherActorLocation - CharacterLocation).Rotator();
	float Pitch = LookRotation.Pitch;
	FRotator TargetRotation;

	const float DistanceToTarget = GetDistanceFromCharacter(OtherActor);
	const float PitchInRange = (DistanceToTarget * PitchDistanceCoefficient + PitchDistanceOffset) * -1.0f;
	const float PitchOffset = FMath::Clamp(PitchInRange, PitchMin, PitchMax);

	Pitch = Pitch + PitchOffset;
	TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);

	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(GameInstanceRef->TimeSlow) return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds() * 2, 9.0f);
	if(GameInstanceRef->TimeStop) return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds() * 5, 9.0f);

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 9.0f);
}

float USATORI_TargetSystemComponent::GetAngleUsingCameraRotation(const AActor* ActorToLook) const
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

float USATORI_TargetSystemComponent::GetAngleUsingCharacterRotation(const AActor* ActorToLook) const
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

	TArray<AActor*> ActorsToLook;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CurrentTarget);

	TArray < AActor* >& AllEnemies = GetWorld()->GetAuthGameMode<ASATORIGameMode>()->GetEnemyActorsRef();

	for (AActor* Actor : AllEnemies)
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
		if (Distance < MinimumDistanceToTarget + AddedDistanceToNotLoseTarget)
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
	
	// Sticky feeling computation
	if (bEnableStickyTarget)
	{
		StartRotatingStack += (AxisValue != 0) ? AxisValue * AxisMultiplier : (StartRotatingStack > 0 ? -AxisMultiplier : AxisMultiplier);

		if (AxisValue == 0 && FMath::Abs(StartRotatingStack) <= AxisMultiplier)
		{
			StartRotatingStack = 0.0f;
		}

		// If Axis value does not exceeds configured threshold, do nothing
		if (FMath::Abs(StartRotatingStack) < StickyRotationThreshold)
		{
			bDesireToSwitch = false;
			return false;
		}

		//Sticky when switching target.
		if (StartRotatingStack * AxisValue > 0)
		{
			StartRotatingStack = StartRotatingStack > 0 ? StickyRotationThreshold : -StickyRotationThreshold;
		}
		else if (StartRotatingStack* AxisValue < 0)
		{
			StartRotatingStack = StartRotatingStack * -1.0f;
		}

		bDesireToSwitch = true;

		return true;
	}


	return FMath::Abs(AxisValue) > StartRotatingThreshold;
}

TArray<AActor*> USATORI_TargetSystemComponent::FindTargetsInRange(TArray<AActor*> ActorsToLook, const float RangeMin, const float RangeMax) const
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

float USATORI_TargetSystemComponent::GetDistanceFromCharacter(const AActor* OtherActor) const
{
	return OwnerActor->GetDistanceTo(OtherActor);
}

void USATORI_TargetSystemComponent::ControlRotation(const bool ShouldControlRotation)
{

	OwnerPawn->bUseControllerRotationYaw = ShouldControlRotation;

	UCharacterMovementComponent* CharacterMovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->bOrientRotationToMovement = !ShouldControlRotation;
	}
}

void USATORI_TargetSystemComponent::ResetIsSwitchingTarget()
{
	bIsSwitchingTarget = false;
	bDesireToSwitch = false;
}

bool USATORI_TargetSystemComponent::GetTargetLockedStatus() const
{
	return bTargetLocked;
}

AActor* USATORI_TargetSystemComponent::GetLockedOnTargetActor() const
{
	return LockedOnTargetActor;
}

bool USATORI_TargetSystemComponent::IsLocked() const
{
	return bTargetLocked && LockedOnTargetActor;
}

void USATORI_TargetSystemComponent::CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor)
{

	TargetLockedOnWidgetComponent = NewObject<UWidgetComponent>(TargetActor, MakeUniqueObjectName(TargetActor, UWidgetComponent::StaticClass(), FName("TargetLockOn")));
	TargetLockedOnWidgetComponent->SetWidgetClass(LockedOnWidgetClass);

	UMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UMeshComponent>();
	USceneComponent* ParentComponent = MeshComponent && LockedOnWidgetParentSocket != NAME_None ? MeshComponent : TargetActor->GetRootComponent();

	if (IsValid(OwnerPlayerController))
	{
		TargetLockedOnWidgetComponent->SetOwnerPlayer(OwnerPlayerController->GetLocalPlayer());
	}

	TargetLockedOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TargetLockedOnWidgetComponent->SetupAttachment(ParentComponent, LockedOnWidgetParentSocket);
	TargetLockedOnWidgetComponent->SetRelativeLocation(LockedOnWidgetRelativeLocation);
	TargetLockedOnWidgetComponent->SetDrawSize(FVector2D(LockedOnWidgetDrawSize, LockedOnWidgetDrawSize));
	TargetLockedOnWidgetComponent->SetVisibility(true);
	TargetLockedOnWidgetComponent->RegisterComponent();
}