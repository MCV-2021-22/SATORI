// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Raijin/SATORI_Raijin.h"

ASATORI_Raijin::ASATORI_Raijin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);

	//AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));


	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Raijin/BT_Raijin.BT_Raijin")));


	//bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/Blueprints/Satori/Player/Enemy/Enemy_BT.Enemy_BT")));
	//btree = bte.LoadSynchronous();

}

void ASATORI_Raijin::BeginPlay()
{
	Super::BeginPlay();

	setSpawnPos(GetActorLocation());


}

void ASATORI_Raijin::setSpawnPos(FVector Pos)
{
	SpawnPos = Pos;
}

FVector ASATORI_Raijin::getSpawnPos()
{
	return SpawnPos;
}

float ASATORI_Raijin::getSoCloseDist()
{
	return SoCloseDist;
}
