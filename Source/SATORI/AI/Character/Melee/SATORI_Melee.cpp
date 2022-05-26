// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Melee/SATORI_Melee.h"

ASATORI_Melee::ASATORI_Melee()
{
	PrimaryActorTick.bCanEverTick = true;

	bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Melee/BT_MELEE.BT_MELEE")));

}
