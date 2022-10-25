//

#include "AI/Gas/GeneralAbilities/SATORI_AIDeathAbility.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "SATORIGameMode.h"
#include "AI/Character/Melee/SATORI_Melee.h"

USATORI_AIDeathAbility::USATORI_AIDeathAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USATORI_AIDeathAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	
	if (!StateTag.IsValid() || !AbilityTag.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AIDeathAbility: Tag is not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AActor* Actor = GetAvatarActorFromActorInfo();
	ASATORI_AICharacter* Character = Cast<ASATORI_AICharacter>(Actor);

	if (!IsValid(Character) || !IsValid(Actor))
	{
		UE_LOG(LogTemp, Display, TEXT("[%s] USATORI_AIDeathAbility: Character or Actor not valid ... "), *GetName());
		Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);

	Character->RemoveGameplayTag(StateTag);
	Character->RemoveGameplayTag(AbilityTag);

	UAnimMontage* AnimMontage = Character->GetCurrentMontage();
	if (IsValid(AnimMontage))
	{
		Character->StopAnimMontage(AnimMontage);
	}

	//Character->EnemyDissolveAfterDeath(1.0f);

	//if (Character->GetEnemyType() == SATORIEnemyType::Melee)
	//{
	//	ASATORI_Melee* Melee = Cast<ASATORI_Melee>(Actor);
	//	if (Melee)
	//	{
	//		Melee->WeaponDissolveAfterDeath();
	//	}
	//}

	Character->GetCharacterMovement()->Velocity = FVector(0);
	Character->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	//Character->SpawnCointActorAfterDeath();
}