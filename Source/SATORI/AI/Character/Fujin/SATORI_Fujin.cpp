// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/Fujin/SATORI_Fujin.h"
// Fill out your copyright notice in the Description page of Project Settings.
#include "SATORICharacter.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"


ASATORI_Fujin::ASATORI_Fujin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	//CharacterMovement->bRunPhysicsWithNoController = true;

	//CharMoveComp

	//uint8 bRunPhysicsWithNoController : 1;

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	//CharacterMovement->bRunPhysicsWithNoController = 3;


	Left = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left"));
	CollisionL = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Left Collision"));
	if (Left)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Left->AttachToComponent(GetMesh(), AttachmentRules, "LHand");
		// Sphere Collision
		CollisionL->SetCapsuleSize(30.f, 50.f, true);
		//CollisionL->SetCollisionProfileName("Pawn");
		CollisionL->SetGenerateOverlapEvents(false);
		CollisionL->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnOverlapLeft);
		CollisionL->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnEndOverlapLeft);
		CollisionL->AttachTo(Left);
	}

	Right = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right"));
	CollisionR = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Right Collision"));
	if (Right)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Right->AttachToComponent(GetMesh(), AttachmentRules, "RHand");
		// Sphere Collision
		CollisionR->SetCapsuleSize(30.f, 50.f, true);
		//CollisionR->SetCollisionProfileName("Pawn");
		CollisionR->SetGenerateOverlapEvents(false);
		CollisionR->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnOverlapRight);
		CollisionR->OnComponentEndOverlap.AddUniqueDynamic(this, &ASATORI_Fujin::OnEndOverlapRight);
		CollisionR->AttachTo(Right);
	}

	

	//{
		//const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		//Nube->AttachToComponent(GetMesh(), AttachmentRules, "BckNubeFujin");

		//UNiagaraFunctionLibrary::SpawnSystemAttached(Nube,GetMesh(), "BckNubeFujin", AttachmentRules);
	//}

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Trueno, RayoLocation);

	BehaviorTree = TSoftObjectPtr <UBehaviorTree>(FSoftObjectPath(TEXT("/Game/SATORI/AI/Fujin/BT_Fujin.BT_Fujin")));
	posinicial = GetActorLocation();
}


void ASATORI_Fujin::BeginPlay()
{
	Super::BeginPlay();

	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Jugable"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fase.Inicial"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Inmune"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Fujin.Esperando"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.InmunityHabs"));


	//(UNiagaraSystem * SystemTemplate, USceneComponent * AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, ENCPoolMethod PoolingMethod, bool bAutoActivate = true, bool bPreCullCheck = true);
	Nube1 = UNiagaraFunctionLibrary::SpawnSystemAttached(Nube, GetMesh(), TEXT("BckNubeFujin"), FVector(0), FRotator::ZeroRotator,FVector(0.1f,0.1f,0.1f), EAttachLocation::Type::KeepRelativeOffset, false,  ENCPoolMethod::None);
	
	/*if (Nube1)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Nube1->AttachToComponent(GetMesh(), AttachmentRules, "BckNubeFujin");
	}*/

	//CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//init de las prob
	for(int i=0;i<3;i++)
	{
		Prob_C1.Add(33.0f);
		Prob_C2.Add(33.0f);
		Prob_C3.Add(33.0f);
		Prob_C4.Add(33.0f);
	}
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASATORI_Fujin::setRaijin, 0.5f, false);



}

TArray<float> ASATORI_Fujin::GetArrayProbs(int Combo)
{
	if(Combo ==1)
	{
		return Prob_C1;
	}
	else if (Combo == 2)
	{
		return Prob_C2;
	}
	else if (Combo == 3)
	{
		return Prob_C3;
	}
	else
	{
		return Prob_C4;
	}


}

void ASATORI_Fujin::SetArrayProbs(int Combo, TArray<float> newArray)
{
	if (Combo == 1)
	{
		Prob_C1 = newArray;
	}
	else if (Combo == 2)
	{
		Prob_C2 = newArray;
	}
	else if (Combo == 3)
	{
		Prob_C3 = newArray;
	}
	else
	{
		Prob_C4 = newArray;
	}


}

float ASATORI_Fujin::getDistAttack()
{
	return dist_attack;
}

float ASATORI_Fujin::getCloseDist()
{
	return close_dist;
}

void ASATORI_Fujin::ActivarInputPlayer()
{
	if(Player)
	{
		if (Player->IsRootComponentMovable())
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			Player->EnableInput(PlayerController);
			

			if (Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed")))
			{
				Player->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed"));
			}
		}
		
	}
}

void ASATORI_Fujin::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);
	/*if (Player)
	{
		if (player_inside_l)
		{
			if (hit_heavy_l)
			{
				float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, dmg_left_huge, Player, DamageGameplayEffect);
				USATORI_BlueprintLibrary::ApplyGameplayEffect(Player, HitGameplayEffect1);
			}
			else if (hit_l)
			{
				float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player, dmg_left_low, Player1, DamageGameplayEffect);
				USATORI_BlueprintLibrary::ApplyGameplayEffect(Player, HitHeavyGameplayEffect1);
			}
		}
	}*/
	


}

void ASATORI_Fujin::OnOverlapLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);
	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);

	if (Player1)
	{
		Player = Player1;
		{
			player_inside_l = true;
			if (Player->IsRootComponentMovable())
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				Player->DisableInput(PlayerController);
				if(!Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed")))
				{
					Player->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed"));
				}
				
			}
		}

		if (golpe_fuerte)
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player1, dmg_left_huge, Player1, DamageGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(Player1, HitGameplayEffect1);
		}
		else
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player1, dmg_left_low, Player1, DamageGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(Player1, HitHeavyGameplayEffect1);
		}
		CollisionL->SetGenerateOverlapEvents(false);
		//CollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		

	}

}

void ASATORI_Fujin::OnOverlapRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);

	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);

	if (Player1)
	{
 		player_inside_r = true;
		Player = Player1;
		if(Player)
		{
			if (Player->IsRootComponentMovable())
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				Player->DisableInput(PlayerController);
				if (!Player->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed")))
				{
					Player->AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Charmed"));
				}
			}
		}
		
		if(golpe_fuerte)
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player1, dmg_right_huge, Player1, DamageGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(Player1, HitGameplayEffect1);
		}
		else
		{
			float dmg_done = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(Player1, dmg_right_low*3, Player1, DamageGameplayEffect);
			USATORI_BlueprintLibrary::ApplyGameplayEffect(Player1, HitHeavyGameplayEffect1);
		}
		CollisionR->SetGenerateOverlapEvents(false);
		
		//CollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	}

}

void ASATORI_Fujin::OnEndOverlapLeft(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);
	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	if(Player1)
	{
		player_inside_l = false;
		
	}
	/*if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockLeft"));
	}*/

}


void ASATORI_Fujin::OnEndOverlapRight(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ASATORI_Fujin* Character = Cast<ASATORI_Fujin>(OtherActor);
	ASATORICharacter* Player1 = Cast<ASATORICharacter>(OtherActor);
	if (Player1)
	{
		player_inside_r = false;

	}

	/*if (!Character)
	{
		canMove = true;
		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.BlockRight"));
	}*/

}


void ASATORI_Fujin::setRaijin()
{
	TSubclassOf<ASATORI_Raijin> classToFind;
	classToFind = ASATORI_Raijin::StaticClass();

	TArray< AActor* > enemigos;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, enemigos);

	if (enemigos.Num() > 0)
	{
		ASATORI_Raijin* Raij = Cast<ASATORI_Raijin>(enemigos[0]);
		if (Raij)
		{
			Raijin = Raij;
		}

	}

}


bool ASATORI_Fujin::getDowned()
{
	return downed;
}

bool ASATORI_Fujin::getDead()
{
	return dead;
}

void ASATORI_Fujin::setDowned(bool dw)
{
	downed = dw;


}

void ASATORI_Fujin::setDead(bool dw)
{
	dead = dw;


}

void ASATORI_Fujin::startCDDowned()
{
	RemoveGameplayTag(FGameplayTag::RequestGameplayTag("State.Downed"));
	AddGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Downed"));
	setDowned(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleDowned, this, &ASATORI_Fujin::revivirTag, CdTimeDowned, false);

}

void ASATORI_Fujin::revivirTag()
{
	if (!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
	{

		RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Boss.Downed"));
		AddGameplayTag(FGameplayTag::RequestGameplayTag("State.Revive"));
	}

}

void ASATORI_Fujin::revivir()
{
	if(!HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead")))
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

bool ASATORI_Fujin::getRaijinDowned()
{
	return Raijin->getDowned();
}

bool ASATORI_Fujin::getGolpeFuerte()
{
	return golpe_fuerte;
}

void ASATORI_Fujin::setGolpeFuerte(bool golpe)
{
	golpe_fuerte = golpe;
}

void ASATORI_Fujin::BossHealthNotifyAbilityChanged()
{
	BossHealthChanges.Broadcast(true);
}