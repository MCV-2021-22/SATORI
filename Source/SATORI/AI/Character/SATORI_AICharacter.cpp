// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Controller.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ASATORI_AICharacter::ASATORI_AICharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = .25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = false;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	//bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Spawner/BT_Spawner.BT_Spawner")));
	//btree = bte.LoadSynchronous();

	
}

// Called when the game starts or when spawned
void ASATORI_AICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();

		AddAICharacterAbilities();
	}
}

void ASATORI_AICharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ASATORI_AICharacter::AddAICharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAbilities)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAbility for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	for (FSATORIGameplayAbilityInfo Ability : DefaultAbilities->Abilities)
	{
		GrantAbilityToPlayer(FGameplayAbilitySpec(Ability.SATORIAbility, 1, static_cast<uint32>(Ability.AbilityKeys), this));
	}
}

void ASATORI_AICharacter::GrantAbilityToPlayer(FGameplayAbilitySpec Ability)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!Ability.Ability)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s() Ability Not Granted for %s. Ability is not valid."), *FString(__FUNCTION__), *GetName());
		return;
	}

	AbilitySystemComponent->GiveAbility(Ability);
}


void ASATORI_AICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Display, TEXT("LLEGAMOS A VER SI SE LLAMA A ESTA FUNCION PADRE"));

	
	if (Cast<AAIController>(NewController) != nullptr) {
		int a = 1;
		//AddGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.AI"));

		UE_LOG(LogTemp, Display, TEXT("EJECUTAMOS EL BT"));

		AAIController* controller = Cast<AAIController>(NewController);

		btree = bte.LoadSynchronous();
		controller->RunBehaviorTree(btree);

		AddAICharacterAbilities();


	}


}

float ASATORI_AICharacter::getDistAttack()
{
	return dist_attack;
}