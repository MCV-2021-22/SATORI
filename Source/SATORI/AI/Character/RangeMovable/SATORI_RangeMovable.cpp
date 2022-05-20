// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/RangeMovable/SATORI_RangeMovable.h"
#include "AI/Components/Arqueros/SATORI_ArcherProjectileSpawnerComponent.h"


ASATORI_RangeMovable::ASATORI_RangeMovable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	

	ProjectileSpawnerComponent = CreateDefaultSubobject<USATORI_ArcherProjectileSpawnerComponent>(TEXT("ProjectileSpawnerComponent"));
}