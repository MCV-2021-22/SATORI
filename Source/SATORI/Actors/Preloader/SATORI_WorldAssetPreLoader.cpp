// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Preloader/SATORI_WorldAssetPreLoader.h"
#include "SATORICharacter.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SATORI/Data/SATORI_AbilityDataAsset.h"
#include "AbilitySystemComponent.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"

// Sets default values
ASATORI_WorldAssetPreLoader::ASATORI_WorldAssetPreLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASATORI_WorldAssetPreLoader::BeginPlay()
{
	Super::BeginPlay();
	
    ApplyDefaultPlayerGameplayAbilities();
}

// Apply Abilities
void ASATORI_WorldAssetPreLoader::ApplyDefaultPlayerGameplayAbilities()
{
    if (!DefaultAbilities)
    {
        UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAbility for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
        return;
    }

    ASATORICharacter* Character = Cast<ASATORICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Character)
    {
        USATORI_AbilitySystemComponent* AbilitySystemComponent = Cast<USATORI_AbilitySystemComponent>(Character->GetAbilitySystemComponent());
        if (AbilitySystemComponent)
        {
            for (FSATORIGameplayAbilityInfo Ability : DefaultAbilities->Abilities)
            {
                AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.SATORIAbility, 1, static_cast<uint32>(Ability.AbilityKeys), Character));
            }
        }
    }
}


