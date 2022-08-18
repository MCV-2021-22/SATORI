//

#include "GAS/Abilities/SATORI_DashAbility.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Character/SATORI_PlayerController.h"
#include "DrawDebugHelpers.h"

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

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbility: Failed commit ability ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	CapsuleComponent = Character->GetCapsuleComponent();
	CapsuleComponent->SetGenerateOverlapEvents(false);
	CapsuleComponent->SetCollisionObjectType(CollisionChannel);

	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());

	ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
	if (Controller)
	{
		Character->DisableInput(Controller);
	}

	DirectionDash = Direction;

	if (!Character->GetVelocity().IsNearlyZero())
	{
		FVector Velocity = Character->GetVelocity();
		FRotator Rotation = Character->GetActorRotation();
		DirectionDash = Rotation.UnrotateVector(Velocity);
		DirectionDash.Normalize();
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
	Character = Cast<ASATORI_CharacterBase>(GetAvatarActorFromActorInfo());
	if(Character)
	{
		CapsuleComponent->SetCollisionObjectType(PlayerChannel);
		CapsuleComponent->SetGenerateOverlapEvents(true);

		ASATORI_PlayerController* Controller = Cast<ASATORI_PlayerController>(Character->GetController());
		if (Controller)
		{
			Character->EnableInput(Controller);
		}
	}
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
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}

void USATORI_DashAbility::Tick(float DeltaTime)
{

	FVector Position = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector End = Position + GetAvatarActorFromActorInfo()->GetVelocity() * 0.5f;
	bool bHitAnything = GetWorld()->LineTraceSingleByChannel(HitResult, Position, End, CollisionChannel, Params);
	if (bHitAnything) 
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	Character->AddActorLocalOffset(DirectionDash * DashSpeed * DeltaTime);
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