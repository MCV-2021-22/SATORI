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
#include "Components/WidgetComponent.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SATORI_PlayerController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ASATORI_Portal::ASATORI_Portal()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SpawnParticleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnParticleComponent"));
	SpawnParticleComponent->SetupAttachment(RootComponent);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);

	// BillboardComponent
	PortalAbilityIconTexture = CreateDefaultSubobject<UBillboardComponent>(TEXT("PortalAbilityIcon"));
	PortalAbilityIconTexture->SetupAttachment(RootComponent);

	PortalEffectIconTexture = CreateDefaultSubobject<UBillboardComponent>(TEXT("PortalEffectIcon"));
	PortalEffectIconTexture->SetupAttachment(RootComponent);

	PortalEffectIconTexture->SetVisibility(false);
	PortalAbilityIconTexture->SetVisibility(false);

	// Collision Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(120.0f);
	//SphereComponent->SetCollisionProfileName(FName("IgnoreAllOverlapOnlyPlayer"));
	SphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_Portal::OnComponentBeginOverlap);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	// Hide interaction
	WidgetComponent->SetVisibility(false, true);

}

// Called when the game starts or when spawned
void ASATORI_Portal::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetAuthGameMode<ASATORIGameMode>()->AddPortalActor(this);
}

void ASATORI_Portal::OnConstruction(const FTransform& Transform)
{
}

void ASATORI_Portal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ActivatePortal();
	//UE_LOG(LogTemp, Warning, TEXT("On Portal"));
}

void ASATORI_Portal::ApplyEffectToPlayer(ASATORICharacter* PlayerCharacter)
{
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

void ASATORI_Portal::GrantedAbilityToPlayer(ASATORICharacter* PlayerCharacter)
{
	FSATORI_AbilitiesDatas AbilityData;
	AbilityData.AbilitiyIcon = PortalAbilityToApply.AbilitiyIcon;
	AbilityData.AbilityName = PortalAbilityToApply.AbilityName;
	AbilityData.CurrentAbility = PortalAbilityToApply.CurrentAbility;
	AbilityData.isUpgrated = PortalAbilityToApply.isUpgrated;

	if (PlayerCharacter)
	{
		PlayerCharacter->GetPlayerAbilityComponent()->AddPortalAbilities(AbilityData);
	}

}

TSubclassOf<UGameplayEffect> ASATORI_Portal::GetCurrentGameplayEffect()
{
	return PortalEffectsToApply.PassiveEffect;
}

void ASATORI_Portal::PlayAnimationAndEffectsPortal_Implementation()
{
}

void ASATORI_Portal::SetCurrentGameplayEffectData(FSATORI_DoorPassiveReward CurrentEffectData)
{
	PortalEffectsToApply = CurrentEffectData;
	CurrentGameplayEffect = PortalEffectsToApply.PassiveEffect;
}

void ASATORI_Portal::SetCurrentGameplayAbilityData(FSATORI_PortalAbilitiesDatasReward CurrentAbilityData)
{
	PortalAbilityToApply = CurrentAbilityData;
	CurrentAbility = PortalAbilityToApply.CurrentAbility;
}

void ASATORI_Portal::ActivatePortal()
{
	IsPortalActive = true;
	//UE_LOG(LogTemp, Display, TEXT("[%s] ASATORI_Portal: Portal is Active ... "), *GetName());
	
	SphereComponent->SetCollisionProfileName(FName("IgnoreAllOverlapOnlyPlayer"));

	if (PortalEffectsToApply.PassiveIcon && PortalAbilityToApply.AbilitiyIcon)
	{
		PortalEffectIconTexture->SetVisibility(true);
		PortalAbilityIconTexture->SetVisibility(true);
		PortalAbilityIconTexture->SetSprite(PortalEffectsToApply.PassiveIcon);
		PortalEffectIconTexture->SetSprite(PortalAbilityToApply.AbilitiyIcon);
	}	
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
		GameInstanceRef->bIsShowingMainWidget = true;

		// Abilities
		GameInstanceRef->NormalAbilities = Character->GetPlayerAbilityComponent()->GetCharacterAbilities();
		GameInstanceRef->UpgratedAbilities = Character->GetPlayerAbilityComponent()->GetCharacterAbilities();
		GameInstanceRef->CurrentPlayerAbilityId = Character->GetPlayerAbilityComponent()->CurrentAbilityValue;
		RemoveGameinstanceAbilities(GameInstanceRef, CurrentId);
	}

	FVector SpawnLocation = Character->GetActorLocation();
	ASATORI_PlayerController* PlayerController = Cast<ASATORI_PlayerController>(Character->GetController());

	SpawnParticleLocation.X = SpawnLocation.X;
	SpawnParticleLocation.Y = SpawnLocation.Y;
	//SpawnParticleLocation.Z = SpawnLocation.Z;

	if (Door_Particles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Door_Particles, SpawnParticleLocation);
	}

	PlayAnimationAndEffectsPortal();

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		Character->DisableInput(PlayerController);
	}

	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (LevelNames.Num() != 0)
			{
				int NumLevels = LevelNames.Num() - 1;
				int Level = FMath::RandRange(0, NumLevels);
				UGameplayStatics::OpenLevel(GetWorld(), FName(LevelNames[Level]));
			}
		}), 1.0f, false);
}

void ASATORI_Portal::Interact(AActor* ActorInteracting)
{
	FVector Delta = ActorInteracting->GetActorLocation() - GetActorLocation();
	//UE_LOG(LogTemp, Display, TEXT("Interact With Door"));

	// Todo : Interact with the player
	ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Character)
	{
		return;
	}

	if (Character && IsPortalActive)
	{
		if (CurrentGameplayEffect.Get())
		{
			ApplyEffectToPlayer(Character);
		}

		if (CurrentAbility.Get())
		{
			GrantedAbilityToPlayer(Character);
		}

		ChangeLevel(Character);
	}
}

void ASATORI_Portal::StartCanInteract(AActor* ActorInteracting)
{
	if(IsPortalActive)
		WidgetComponent->SetVisibility(true, true);
}

void ASATORI_Portal::StopCanInteract(AActor* ActorInteracting)
{
	if (IsPortalActive)
		WidgetComponent->SetVisibility(false, true);
}

void ASATORI_Portal::SetCurrentId(int Id)
{
	CurrentId = Id;
}

void ASATORI_Portal::RemoveGameinstanceAbilities(USATORI_GameInstance* GameInstanceRef , int Id)
{
	if (Id != 0 && GameInstanceRef)
	{
		ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!Character->GetIsAbilityUpgrated())
		{
			GameInstanceRef->RemoveElementonFromNormalAbilities(Id);
		}
		else
		{
			GameInstanceRef->RemoveElementonFromUpgratedAbilities(Id);
		}
	}
}

void ASATORI_Portal::ResetCurrentPortalData()
{
	// Remove Current reward
	CurrentGameplayEffect = nullptr;
	CurrentAbility = nullptr;

	PortalAbilityIconTexture->SetSprite(nullptr);
	PortalEffectIconTexture->SetSprite(nullptr);

	PortalEffectsToApply.PassiveEffect = nullptr;
	PortalEffectsToApply.PassiveIcon = nullptr;
	PortalEffectsToApply.Description = FText::GetEmpty();

	PortalAbilityToApply.CurrentAbility = nullptr;
	PortalAbilityToApply.AbilitiyIcon = nullptr;
	PortalAbilityToApply.AbilityName = FText::GetEmpty();
	PortalAbilityToApply.isUpgrated = false;
}

void ASATORI_Portal::SetCurrentMeshInteractability(bool CanInteract)
{
	//StaticMeshComponent->Co
	if (CanInteract)
	{
		//StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
		//UE_LOG(LogTemp, Warning, TEXT("TRUE!!!!!!!!!!!!!!!!!"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("FALSE!!!!!!!!!!!!!!!!!"));
		//StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}
}