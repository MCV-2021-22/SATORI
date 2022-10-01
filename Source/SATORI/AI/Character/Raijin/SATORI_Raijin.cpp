// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Raijin/SATORI_Raijin.h"

#include "Kismet/GameplayStatics.h"

ASATORI_Raijin::ASATORI_Raijin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);

	//AttributeSet = CreateDefaultSubobject<USATORI_AttributeSet>(TEXT("AttributeSet"));


	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Raijin/BT_Raijin.BT_Raijin")));
	SwordComponentRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordRight"));

	//bte = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/Blueprints/Satori/Player/Enemy/Enemy_BT.Enemy_BT")));
	//btree = bte.LoadSynchronous();
	if (SwordComponentRight)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponentRight->AttachToComponent(GetMesh(), AttachmentRules, "BajoSocket");
		// Sphere Collision
	}
}

void ASATORI_Raijin::BeginPlay()
{
	Super::BeginPlay();

	setSpawnPos(GetActorLocation());

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &ASATORI_Raijin::setArcoAltavoces, 0.5f, false);


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

void ASATORI_Raijin::setArcoAltavoces()
{
	TSubclassOf<ASATORI_ArcoAltavoces> classToFind;
	classToFind = ASATORI_ArcoAltavoces::StaticClass();

	TArray< AActor* > enemigos;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, enemigos);

	if(enemigos.Num() >0)
	{
		ASATORI_ArcoAltavoces* Speaker = Cast<ASATORI_ArcoAltavoces>(enemigos[0]);
		if(Speaker)
		{
			ArcoAltavoces = Speaker;
		}

	}

}
