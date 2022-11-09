//

#include "GAS/Abilities/SATORI_DashAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Character/SATORI_PlayerController.h"
#include "DrawDebugHelpers.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GAS/SATORI_AbilitySystemComponent.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"

USATORI_DashAbility::USATORI_DashAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_DashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontageDashForward) || !IsValid(AnimMontageDashBackwards) || !IsValid(AnimMontageDashRight) || !IsValid(AnimMontageDashLeft))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_DashAbility: Cannot get Animation Montage ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagEndAbility.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Cast ASATORICharacter ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	PlayerCharacter = Cast<ASATORICharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Cannot Cast ASATORICharacter ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	CapsuleComponent = Character->GetCapsuleComponent();
	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());

	if (!Character->GetVelocity().IsNearlyZero(20.0f))
	{
		FVector Velocity = Character->GetVelocity();
		FRotator Rotation = Character->GetActorRotation();
		DirectionDash = Rotation.UnrotateVector(Velocity);
		DirectionDash.Normalize();
	}
	else
	{
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	bDashing = false;
	bBraking = false;
	SpeedBraking = 0;

	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->DisableInput(Controller);
	}

	float DotProduct = FVector::DotProduct(Direction, DirectionDash);

	if (DotProduct > 0.5f)
	{
		USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontageDashForward, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
		Task->ReadyForActivation();
	}
	else if (DotProduct < -0.5f)
	{
		USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontageDashBackwards, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
		Task->ReadyForActivation();
	}
	else if (DirectionDash.Y > 0 )
	{
		USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontageDashRight, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
		Task->ReadyForActivation();
	}
	else if (DirectionDash.Y < 0)
	{
		USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontageDashLeft, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &USATORI_DashAbility::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &USATORI_DashAbility::OnCancelled);
		Task->EventReceived.AddDynamic(this, &USATORI_DashAbility::EventReceived);
		Task->ReadyForActivation();
	}
}

void USATORI_DashAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	CapsuleComponent->SetGenerateOverlapEvents(true);

	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->EnableInput(Controller);
	}

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_DashAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_DashAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagEndAbility)
	{
		bDashing = false;
	}

	if (EventTag == TagStartAbility)
	{
		CapsuleComponent->SetGenerateOverlapEvents(false);
		bDashing = true;

		FTimerHandle TimerHandle;
		USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GameInstanceRef->TimeSlow)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USATORI_DashAbility::EndDash, TimeToFinish/2, false);
		}
		else if (GameInstanceRef->TimeStop)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USATORI_DashAbility::EndDash, TimeToFinish/5, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USATORI_DashAbility::EndDash, TimeToFinish, false);
		}
	}

}

void USATORI_DashAbility::EndDash()
{
	bBraking = true;

	PlayerCharacter->GetComboSystemComponent()->isAbilityCanceled = false;

	if (USkeletalMeshComponent* Mesh = Character->GetMesh())
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			AnimInstance->Montage_JumpToSection(FName("EndDash"), AnimInstance->GetCurrentActiveMontage());
		}
	}
}

void USATORI_DashAbility::Tick(float DeltaTime)
{

	if(IsValid(Character))
	{

		FVector Position = Character->GetActorLocation();
	
		//Movement 
		if (bDashing)
		{
			FVector End = Position;
			End.Z -= 150.0f;
			bool bHitAnything = GetWorld()->LineTraceSingleByProfile(OutHit, Position, End, FName("BlockOnlyStatic"), CollisionParams);
			if(bHitAnything)
				Position.Z = Position.Z + 10.0f;
			Character->SetActorLocation(Position);
			Character->AddActorLocalOffset(DirectionDash * DashSpeed * DeltaTime, true);
		}

		if (bBraking)
		{
			SpeedBraking += DeltaTime * SpeedBrakingFactor;
			Character->AddActorLocalOffset(DirectionDash * (DashSpeed/SpeedBraking) * DeltaTime, true);
		}
	}
}

bool USATORI_DashAbility::IsTickable() const
{
	return IsActive();
}

bool USATORI_DashAbility::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_DashAbility::GetStatId() const
{
	return UObject::GetStatID();
}