
#include "Components/TargetSystem/SATORI_TargetSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

//Debug
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
USATORI_TargetSystemComponent::USATORI_TargetSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MinimumDistanceToTarget = 600.0f;
	TargetActorsWithTag = FName(TEXT("Enemy"));
	TargetableCollisionChannel = ECollisionChannel::ECC_Pawn;

	TargetLockedOnWidgetComponent = nullptr;

}

// Called when the game starts
void USATORI_TargetSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner(); 
	if (!OwnerActor)
	{
		return;
	}

	OwnerPawn = Cast<APawn>(OwnerActor);
	if (!ensure(OwnerPawn) || !IsValid(OwnerPawn))
	{
		return;
	}
	
	OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());

}

void USATORI_TargetSystemComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bTargetLocked || !LockedOnTargetActor)
	{
		return;
	}

	if (!LockedOnTargetActor->ActorHasTag("Enemy")) //Gestion de Tags
	{
		TargetLockOff();
		return;
	}

	SetControlRotationOnTarget(LockedOnTargetActor);

	if (GetDistanceFromCharacter(LockedOnTargetActor) > MinimumDistanceToTarget)
	{
		TargetLockOff();
	}

	if (ShouldBreakLineOfSight() && !bIsBreakingLineOfSight)
	{
		if (BreakLineOfSightDelay <= 0)
		{
			TargetLockOff();
		}
		else
		{
			bIsBreakingLineOfSight = true;
			GetWorld()->GetTimerManager().SetTimer(
				LineOfSightBreakTimerHandle,
				this,
				&USATORI_TargetSystemComponent::BreakLineOfSight,
				BreakLineOfSightDelay
			);
		}
	}

	//if(IsValid(LockedOnTargetActor))
	//DrawDebugLine(
	//GetWorld(),
	//OwnerActor->GetActorLocation(),
	//LockedOnTargetActor->GetActorLocation(),
	//FColor(255, 0, 0),
	//false, 1.0f, 0,
	//1
	//);


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
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetActorsWithTag, Actors);
		LockedOnTargetActor = FindNearestTarget(Actors);
		TargetLockOn(LockedOnTargetActor);
	}

}

void USATORI_TargetSystemComponent::TargetLockOff()
{
	bTargetLocked = false;

	//TargetLockedOnWidgetComponent->DestroyComponent();

	//ControlRotation(false);

	OwnerPlayerController->ResetIgnoreLookInput();

	LockedOnTargetActor = nullptr;

}

void USATORI_TargetSystemComponent::TargetLockOn(AActor* TargetToLockOn)
{
	if (!IsValid(TargetToLockOn))
	{
		return;
	}

	bTargetLocked = true;

	OwnerPlayerController->SetIgnoreLookInput(true);

	//CreateAndAttachTargetLockedOnWidgetComponent(TargetToLockOn);

	//ControlRotation(true);
	

}

bool USATORI_TargetSystemComponent::GetTargetLockedStatus()
{
	return false;
}

AActor* USATORI_TargetSystemComponent::GetLockedOnTargetActor()
{
	return LockedOnTargetActor;
}

bool USATORI_TargetSystemComponent::IsLocked()
{
	return false;
}

TArray<AActor*> USATORI_TargetSystemComponent::FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax)
{
	return TArray<AActor*>();
}

AActor* USATORI_TargetSystemComponent::FindNearestTarget(TArray<AActor*> Actors)
{
	TArray<AActor*> ActorsHit;
	//UE_LOG(LogTemp, Display, TEXT("FindNearestTarget"));
	for (AActor* Actor : Actors)
	{
		//UE_LOG(LogTemp, Display, TEXT("Actor: %s"), *Actor->GetName());
		bool bHit = LineTraceForActor(Actor);
		//UE_LOG(LogTemp, Log, TEXT("bHit: %s"), bHit ? TEXT("true") : TEXT("false"));
		if (bHit && IsInViewport(Actor))
		{
			ActorsHit.Add(Actor);
		}
	}

	if (ActorsHit.Num() == 0)
	{
		return nullptr;
	}

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

	//if(IsValid(Target))
	//DrawDebugLine(
	//GetWorld(),
	//OwnerActor->GetActorLocation(),
	//Target->GetActorLocation(),
	//FColor(255, 0, 0),
	//false, 1.0f, 0,
	//1
	//);

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
	//DrawDebugLine(
	//	GetWorld(),
	//	OwnerActor->GetActorLocation(),
	//	OtherActor->GetActorLocation(),
	//	FColor(255, 0, 0),
	//	false, 1.0f, 0,
	//	1
	//);
	//UE_LOG(LogTemp, Display, TEXT("LineTraceForActor"));
	//UE_LOG(LogTemp, Display, TEXT("Actor: %s"), *OtherActor->GetName());
	//UE_LOG(LogTemp, Log, TEXT("bHit: %s"), bHit ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Display, TEXT("Hit Result Actor: %s"), *HitResult.GetActor()->GetName());
	if (bHit)
	{
		if (HitResult.GetActor() == OtherActor)
		{
			//DrawDebugLine(
			//	GetWorld(),
			//	OwnerActor->GetActorLocation(),
			//	OtherActor->GetActorLocation(),
			//	FColor(0, 255, 0),
			//	false, 1.0f, 0,
			//	1
			//);
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

	bool bHit = LineTraceForActor(LockedOnTargetActor);

	if (bHit)
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

float USATORI_TargetSystemComponent::GetDistanceFromCharacter(const AActor* OtherActor)
{
	return OwnerActor->GetDistanceTo(OtherActor);
}

FRotator USATORI_TargetSystemComponent::GetControlRotationOnTarget(const AActor* OtherActor)
{
	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();

	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector OtherActorLocation = OtherActor->GetActorLocation();

	// Find look at rotation
	const FRotator LookRotation = FRotationMatrix::MakeFromX(OtherActorLocation - CharacterLocation).Rotator();
	float Pitch = LookRotation.Pitch;
	FRotator TargetRotation;

	TargetRotation = FRotator(ControlRotation.Pitch, LookRotation.Yaw, ControlRotation.Roll);

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 9.0f);
}

void USATORI_TargetSystemComponent::SetControlRotationOnTarget(AActor* TargetActor)
{

	FRotator ControlRotation = GetControlRotationOnTarget(TargetActor);
	OwnerPlayerController->SetControlRotation(ControlRotation);

}

void USATORI_TargetSystemComponent::ControlRotation(bool ControlRotation)
{
	UCharacterMovementComponent* CharacterMovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->bOrientRotationToMovement = !ControlRotation;
	}
}

float USATORI_TargetSystemComponent::GetAngleUsingCameraRotation(const AActor* ActorToLook)
{
	return 0.0f;
}

float USATORI_TargetSystemComponent::GetAngleUsingCharacterRotation(const AActor* ActorToLook)
{
	return 0.0f;
}

FRotator USATORI_TargetSystemComponent::FindLookAtRotation(const FVector Start, const FVector Target)
{
	return FRotator();
}

void USATORI_TargetSystemComponent::CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor)
{
}

