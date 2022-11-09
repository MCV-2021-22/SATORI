//

#include "AI/Gas/Melee/SATORI_AI_DashAbilityMelee.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "Character/SATORI_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

USATORI_AI_DashAbilityMelee::USATORI_AI_DashAbilityMelee()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_AI_DashAbilityMelee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if (!IsValid(AnimMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] USATORI_AI_DashAbilityMelee: Cannot get Animation Montage ... "), *GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Melee = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (!Melee)
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_DashAbilityMelee: Cannot Cast ASATORICharacter ... "), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TagSpawnAbility.IsValid() || !TagEndAbility.IsValid() || !TagStartDash.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AI_DashAbilityMelee: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	bDashing = false;

	//Handling of events
	USATORI_PlayMontageAndWaitEvent* Task = USATORI_PlayMontageAndWaitEvent::PlayMontageAndWaitForEvent(this, NAME_None, AnimMontage, FGameplayTagContainer(), 1.0f, NAME_None, bStopWhenAbilityEnds, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &USATORI_AI_DashAbilityMelee::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AI_DashAbilityMelee::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_AI_DashAbilityMelee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Melee->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop"));
	DashActor->Destroy();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_AI_DashAbilityMelee::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_AI_DashAbilityMelee::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_AI_DashAbilityMelee::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag == TagSpawnAbility)
	{
		GetTarget();
	}

	//Starts Dashing towards DashActor
	if (EventTag == TagStartDash)
	{
		bTargeting = false;		
		DashActorPosition = DashActor->GetActorLocation();
	
		//CheckVisibility
		FHitResult HitResult;
		FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
		Params.AddIgnoredActor(Melee);
		FVector DashActorVisibility = DashActorPosition;
		DashActorVisibility.Z = DashActorVisibility.Z + 50;
		//DrawDebugSphere(GetWorld(), DashActorVisibility, 10.0f, 12, FColor(255, 0, 0), false, 10.0f);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Melee->GetActorLocation(), DashActorVisibility, ECollisionChannel::ECC_Visibility, Params);
		if (bHit)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		bDashing = true;
	}
}

//Targets Player/Clone
void USATORI_AI_DashAbilityMelee::GetTarget()
{
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetAvatarActorFromActorInfo());
	if (IsValid(Blackboard))
	{
		FName Player = "Target";
		ASATORI_CharacterBase* Target = Cast<ASATORI_CharacterBase>(Blackboard->GetValueAsObject(Player));

		if (IsValid(Target))
		{
			Enemy = Target;
			SpawnActor();
		}
	}
}

void USATORI_AI_DashAbilityMelee::SpawnActor()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Enemy->GetActorLocation());

	//Decal creation
	DashActor = GetWorld()->SpawnActorDeferred<ASATORI_DashMeleeActor>(DashMeleeActor, SpawnTransform, GetOwningActorFromActorInfo(), Melee, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	DashActor->DamageGameplayEffect = DamageGameplayEffect;
	DashActor->Damage = Damage;
	DashActor->OwnerMelee = GetAvatarActorFromActorInfo();
	DashActor->FinishSpawning(SpawnTransform);

	bTargeting = true;
}

void USATORI_AI_DashAbilityMelee::Tick(float DeltaTime)
{

	if (!IsValid(Melee))
	{
		return;
	}

	//Follow for a brief moment player moment and rotate facing target
	if (bTargeting)
	{
		FVector Position = DashActor->GetActorLocation();
		FVector NextPosition = UKismetMathLibrary::VInterpTo(Position, Enemy->GetActorLocation(), DeltaTime, 10.0f);
		DashActor->SetActorLocation(NextPosition);

		FRotator MeleeRotation = (Position - Melee->GetActorLocation()).ToOrientationRotator();
		MeleeRotation.Pitch = 0;
		Melee->SetActorRotation(MeleeRotation);
	}

	//Dash movement
	if(bDashing)
	{
		FVector Position = Melee->GetActorLocation();
		FVector NextPosition = UKismetMathLibrary::VInterpTo(Position, DashActorPosition, DeltaTime, DashSpeed);
		Melee->SetActorLocation(NextPosition);
	}

	//Stop dashing - It activates when actorreaches collision box in DashActor 
	if(Melee->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop")))
	{
		bDashing = false;
	}
}

bool USATORI_AI_DashAbilityMelee::IsTickable() const
{
	return IsActive();
}

bool USATORI_AI_DashAbilityMelee::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_AI_DashAbilityMelee::GetStatId() const
{
	return UObject::GetStatID();
}