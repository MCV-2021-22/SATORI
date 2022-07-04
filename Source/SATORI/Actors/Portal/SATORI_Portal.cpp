// Fill out your copyright notice in the Description page of Project Settings.


#include "SATORI_Portal.h"
#include "SATORI/SATORICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SATORI/Character/SATORI_PlayerState.h"
#include "UI/WorldActor/SATORI_DoorInteractUI.h"
#include "UI/SATORI_MainUI.h"
#include "Data/SATORI_PortalPassiveDataAsset.h"
#include "Components/BillboardComponent.h"
#include <algorithm>
#include "SATORIGameMode.h"
#include "GameplayFramework/SATORI_GameInstance.h"

// Sets default values
ASATORI_Portal::ASATORI_Portal()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);

	PortalIconTexture = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	PortalIconTexture->SetupAttachment(RootComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(120.0f);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Portal::OnComponentBeginOverlap);

}

// Called when the game starts or when spawned
void ASATORI_Portal::BeginPlay()
{
	Super::BeginPlay();
	
	//if (PassiveDataAsset)
	//{
	//	for (const FSATORI_DoorPassiveDatas Data : PassiveDataAsset->PassiveRewards)
	//	{
	//		FString CurrentName = Data.Desciption.ToString();
	//		FName LocalAbilityName = FName(*CurrentName);

	//		if (LocalAbilityName.IsValid() && Data.PassiveEffect)
	//		{
	//			// Adding datas to map
	//			FSATORI_DoorPassiveReward PassiveReward;
	//			PassiveReward.PassiveEffect = Data.PassiveEffect;
	//			PassiveReward.PassiveIcon = Data.PassiveIcon;
	//			PassiveReward.Desciption = Data.Desciption;
	//			PortalEffectsToApply.Add(PassiveReward);
	//		}
	//	}
	//}

	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->AddPortalActor(this);
}

void ASATORI_Portal::OnConstruction(const FTransform& Transform)
{
}

void ASATORI_Portal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Overlap Beg %s"), *OtherActor->GetName());

	ASATORICharacter* Character = Cast<ASATORICharacter>(OtherActor);

	if (!Character)
	{
		return;
	}

	if (Character)
	{
		//DoorInteractUI = CreateWidget<USATORI_DoorInteractUI>(GetGameInstance(), SATORIMainUI);
		//if (DoorInteractUI)
		//{
		//	//DoorInteractUI->AddToViewport();
		//}
		
		// TODO

		

		ApplyEffectToPlayer(Character);

		ChangeLevel(Character);

	}
}

void ASATORI_Portal::ApplyEffectToPlayer(AActor* PlayerActor)
{
	ASATORICharacter* PlayerCharacter = Cast<ASATORICharacter>(PlayerActor);
	if (PlayerCharacter)
	{
		UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(CurrentGameplayEffect,
				PlayerCharacter->GetCharacterLevel(), EffectContext);

			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
					AbilitySystemComponent);
			}
		}
	}
}

TSubclassOf<UGameplayEffect> ASATORI_Portal::GetCurrentGameplayEffect()
{
	return PortalEffectsToApply.PassiveEffect;
}



void ASATORI_Portal::SetCurrentGameplayEffectData(FSATORI_DoorPassiveReward CurrentEffectData)
{
	PortalEffectsToApply = CurrentEffectData;
	CurrentGameplayEffect = PortalEffectsToApply.PassiveEffect;
}

void ASATORI_Portal::ActivatePortal()
{
	Active = true;
	UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_Portal: Portal is Active ... "), *GetName());
	
	//TO DO: UI

	SphereComponent->SetCollisionProfileName(FName("IgnoreAllOverlapOnlyPlayer"));

	PortalIconTexture->SetSprite(PortalEffectsToApply.PassiveIcon);

}

void ASATORI_Portal::ChangeLevel(ASATORICharacter* Character)
{

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstanceRef)
	{
		GameInstanceRef->Health = Character->GetHealth();
		GameInstanceRef->MaxHealth = Character->GetMaxHealth();
		GameInstanceRef->Mana = Character->GetMana();
		GameInstanceRef->MaxMana = Character->GetMaxMana();
		GameInstanceRef->Defense = Character->GetDefense();
		GameInstanceRef->Attack = Character->GetAttack();
		GameInstanceRef->MoveSpeed = Character->GetMoveSpeed();
		GameInstanceRef->Gold = Character->GetGold();
	}

	if (LevelNames.Num() != 0)
	{
		int NumLevels = LevelNames.Num() - 1;
		int  Level = FMath::RandRange(0, NumLevels);
		UGameplayStatics::OpenLevel(GetWorld(), FName(LevelNames[Level]));
	}
	
}