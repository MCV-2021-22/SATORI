//

#include "AI/Gas/Spawn/SATORI_AI_DashAbilitySpawned.h"
#include "AbilitySystemComponent.h"
#include "SATORICharacter.h"
#include "Actors/AbilitiesActors/SATORI_DashMeleeActor.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

USATORI_AI_DashAbilitySpawned::USATORI_AI_DashAbilitySpawned()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsCreateOnRunning = GIsRunning;
}

void USATORI_AI_DashAbilitySpawned::ActivateAbility(
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

	Spawned = Cast<ASATORI_AICharacter>(GetAvatarActorFromActorInfo());
	if (!Spawned)
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
	//Task->OnBlendOut.AddDynamic(this, &USATORI_AI_DashAbilitySpawned::OnCompleted);
	//Task->OnCompleted.AddDynamic(this, &USATORI_AI_DashAbilitySpawned::OnCompleted);
	//Task->OnInterrupted.AddDynamic(this, &USATORI_AI_DashAbilitySpawned::OnCancelled);
	//Task->OnCancelled.AddDynamic(this, &USATORI_AI_DashAbilitySpawned::OnCancelled);
	Task->EventReceived.AddDynamic(this, &USATORI_AI_DashAbilitySpawned::EventReceived);
	Task->ReadyForActivation();
}

void USATORI_AI_DashAbilitySpawned::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Spawned->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop"));
	DashActor->Destroy();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USATORI_AI_DashAbilitySpawned::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USATORI_AI_DashAbilitySpawned::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USATORI_AI_DashAbilitySpawned::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
		Params.AddIgnoredActor(Spawned);
		FVector DashActorVisibility = DashActorPosition;
		DashActorVisibility.Z = DashActorVisibility.Z + 50;
		//DrawDebugSphere(GetWorld(), DashActorVisibility, 10.0f, 12, FColor(255, 0, 0), false, 10.0f);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Spawned->GetActorLocation(), DashActorVisibility, ECollisionChannel::ECC_Visibility, Params);
		if (bHit)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		bDashing = true;
	}
}

//Targets Player/Clone
void USATORI_AI_DashAbilitySpawned::GetTarget()
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

void USATORI_AI_DashAbilitySpawned::SpawnActor()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Enemy->GetActorLocation());

	//Decal creation
	DashActor = GetWorld()->SpawnActorDeferred<ASATORI_DashSpawnedActor>(DashSpawnedActor, SpawnTransform, GetOwningActorFromActorInfo(), Spawned, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	DashActor->DamageGameplayEffect = DamageGameplayEffect;
	DashActor->Damage = Damage;
	DashActor->OwnerSpawned = GetAvatarActorFromActorInfo();
	DashActor->FinishSpawning(SpawnTransform);

	bTargeting = true;
}

void USATORI_AI_DashAbilitySpawned::Tick(float DeltaTime)
{
	//Follow for a brief moment player moment and rotate facing target
	if (bTargeting)
	{
		FVector Position = DashActor->GetActorLocation();
		FVector NextPosition = UKismetMathLibrary::VInterpTo(Position, Enemy->GetActorLocation(), DeltaTime, 10.0f);
		DashActor->SetActorLocation(NextPosition);

		FRotator MeleeRotation = (Position - Spawned->GetActorLocation()).ToOrientationRotator();
		MeleeRotation.Pitch = 0;
		Spawned->SetActorRotation(MeleeRotation);
	}

	//Dash movement
	if(bDashing)
	{
		FVector Position = Spawned->GetActorLocation();
		FVector NextPosition = UKismetMathLibrary::VInterpTo(Position, DashActorPosition, DeltaTime, DashSpeed);
		Spawned->SetActorLocation(NextPosition);
	}

	//Stop dashing - It activates when actorreaches collision box in DashActor 
	if(Spawned->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Dash.Stop")))
	{
		bDashing = false;
	}
}

bool USATORI_AI_DashAbilitySpawned::IsTickable() const
{
	return IsActive();
}

bool USATORI_AI_DashAbilitySpawned::IsAllowedToTick() const
{
	return true;
}

TStatId USATORI_AI_DashAbilitySpawned::GetStatId() const
{
	return UObject::GetStatID();
}