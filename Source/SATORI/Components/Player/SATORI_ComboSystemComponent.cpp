// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "SATORICharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AbilitySystemComponent.h"
#include "Character/SATORI_PlayerController.h"
#include "UI/SATORI_MainUI.h"

// Sets default values for this component's properties
USATORI_ComboSystemComponent::USATORI_ComboSystemComponent()
{
	// Weapon Component
	bWantsInitializeComponent = true;

	CurrentComboState = EComboState::None;

}

void USATORI_ComboSystemComponent::InitializeComponent()
{
	//SwordComponent = NewObject<USkeletalMeshComponent>(GetOwner(), TEXT("Sword"));
	//AttackingCollision = NewObject<USphereComponent>(GetOwner(), TEXT("Collision"));
	//
	///*SwordComponent->SkeletalMesh =  */

	//ASATORICharacter* Character = Cast<ASATORICharacter>(GetOwner());
	//if (SwordComponent)
	//{
	//	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	//	SwordComponent->SetupAttachment(Character->GetMesh(), "Sword_1");
	//	// Sphere Collision
	//	AttackingCollision->InitSphereRadius(40.0f);
	//	AttackingCollision->SetCollisionProfileName("Pawn");
	//	AttackingCollision->SetGenerateOverlapEvents(false);
	//	AttackingCollision->AttachTo(SwordComponent);
	//}

	//SwordComponent->CreationMethod = EComponentCreationMethod::Instance;
	//AttackingCollision->CreationMethod = EComponentCreationMethod::Instance;

	//SwordComponent->RegisterComponent();
	//AttackingCollision->RegisterComponent();

}

// Called when the game starts
void USATORI_ComboSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USATORI_ComboSystemComponent::LigthAttack()
{
	if (isLightAttack)
	{
		isHeavyAttack = false;
		//isLightAttack = false;
		if (lightAttackCounter >= LightAttackMontageToPlay.Num() - 1)
		{
			lightAttackCounter = 0;
		}
		else
		{
			lightAttackCounter ++;
		}
	}
}

void USATORI_ComboSystemComponent::HeavyAttack()
{

}

void USATORI_ComboSystemComponent::ResetAllAttribute()
{
	lightAttackCounter = 0;
	HeavyAttackCounter = 0;
}

bool USATORI_ComboSystemComponent::CanComboAttack()
{
	if (isLightAttack || isHeavyAttack)
		return true;

	return false;
}

void USATORI_ComboSystemComponent::SetComboState(EComboState State)
{
	CurrentComboState = State;
}

void USATORI_ComboSystemComponent::SetMainWidgetVisibility(bool ShowVisibility)
{
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(GetOwner());
	ESlateVisibility EVisibility = ESlateVisibility::Hidden;
	if (PlayerCharacter)
	{
		ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			USATORI_MainUI* MainUI = PlayerController->GetSatoriMainUI();
			if (MainUI)
			{
				if (ShowVisibility)
				{
					EVisibility = ESlateVisibility::Visible;
					MainUI->SetVisibility(EVisibility);
				}
				else if (!ShowVisibility)
				{
					EVisibility = ESlateVisibility::Hidden;
					MainUI->SetVisibility(EVisibility);
				}
			}
		}
	}
}

void USATORI_ComboSystemComponent::ApplyKnockBackTagToEnemy(ASATORI_AICharacter* Enemy)
{
	if (Enemy && CurrentComboState == EComboState::HeavyAttack)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Adding Tags");

		Enemy->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.KnockBack")));

		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("State.KnockBack.Start"));
		Enemy->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("State.KnockBack.Start")), &EventData);

		if (Enemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.KnockBack"))))
		{
			return;
		}
	}
}

void USATORI_ComboSystemComponent::BossHealthNotifyAbilityChanged()
{
	BossHealthChanges.Broadcast(isInBossFight);
}