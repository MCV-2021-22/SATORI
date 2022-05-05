// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Spawner/SATORI_Spawner.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Controller.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "Data/SATORI_AbilityDataAsset.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"



ASATORI_Spawner::ASATORI_Spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);

	//AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));


	bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Spawner/BT_Spawner.BT_Spawner")));


	//bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/Blueprints/Satori/Player/Enemy/Enemy_BT.Enemy_BT")));
	//btree = bte.LoadSynchronous();

}

