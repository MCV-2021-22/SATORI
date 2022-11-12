// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
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

	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Jugable"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fase.Inicial"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.InmunityHabs"));

	setSpawnPos(GetActorLocation());

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &ASATORI_Raijin::setArcoAltavoces, 0.5f, false);

	GetWorld()->GetTimerManager().SetTimer(TimerHandleFujin,this, &ASATORI_Raijin::setFujin, 0.5f, false);


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

float ASATORI_Raijin::getSoCloseDistExt()
{
	return SoCloseDistExt;
}

void ASATORI_Raijin::setNumRayosExtensibles(int num)
{
	num_rayos_extensibles = num;
}

int ASATORI_Raijin::getNumRayosExtensibles()
{
	return num_rayos_extensibles;
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

void ASATORI_Raijin::setFujin()
{
	TSubclassOf<ASATORI_Fujin> classToFind;
	classToFind = ASATORI_Fujin::StaticClass();

	TArray< AActor* > enemigos;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, enemigos);

	if (enemigos.Num() > 0)
	{
		ASATORI_Fujin* Fuj = Cast<ASATORI_Fujin>(enemigos[0]);
		if (Fuj)
		{
			Fujin = Fuj;
			AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Turno.Raijin"));
		}

	}

}

void ASATORI_Raijin::setDowned(bool dw)
{
	downed = dw;

	
}

void ASATORI_Raijin::setDead(bool dw)
{
	dead = dw;


}

bool ASATORI_Raijin::getDowned()
{
	return downed;
}

bool ASATORI_Raijin::getDead()
{
	return dead;
}
void ASATORI_Raijin::startCDDowned()
{

	RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Downed"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Downed"));
	setDowned(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleDowned, this, &ASATORI_Raijin::revivirTag, CdTimeDowned, false);

}

void ASATORI_Raijin::revivirTag()
{
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
	{
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Downed"));
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Revive"));
	}
}

void ASATORI_Raijin::revivir()
{
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
	{
		setDowned(false);
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Revive"));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleDowned);

		SetHealth(GetMaxHealth() * 0.25f);
		AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Jugable"));

		// Notify
		BossHealthNotifyAbilityChanged();
	}
}

bool ASATORI_Raijin::getFujinDowned()
{
	return Fujin->getDowned();
}

void ASATORI_Raijin::StartPartArco()
{
	if (ArcoPart && ArcoAltavoces)
	{
		ParticleEffectArco = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ArcoPart, ArcoAltavoces->GetActorLocation() + FVector(-250, 0, 150));
	}

}

void ASATORI_Raijin::EndPartArco()
{
	if (ParticleEffectArco)
	{
		ParticleEffectArco->DestroyComponent();
	}

}

void ASATORI_Raijin::BossHealthNotifyAbilityChanged()
{
	BossHealthChanges.Broadcast(true);
}