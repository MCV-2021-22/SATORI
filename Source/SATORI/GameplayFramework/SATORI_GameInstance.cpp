// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayFramework/SATORI_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Data/SATORI_PortalGrantedAbilityAsset.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

USATORI_GameInstance::USATORI_GameInstance()
{

}

void USATORI_GameInstance::Init()
{   
	Super::Init();
	InitSaveGame();
	FillPortalGrantedAbilityWithData();

    // Loading Screen
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USATORI_GameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USATORI_GameInstance::EndLoadingScreen);
}

void USATORI_GameInstance::Shutdown()
{
	Super::Shutdown();
	SaveGameToDisk();
}

void USATORI_GameInstance::BeginLoadingScreen(const FString& MapName)
{
    if (!IsRunningDedicatedServer())
    {
        if (isPlayingCinematic)
        {
            isPlayingCinematic = false;
            FLoadingScreenAttributes LoadingScreen;
            LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
            LoadingScreen.bWaitForManualStop = false;

            LoadingWidget = CreateWidget<UUserWidget>(this, WidgetTemplate);

            TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();
            LoadingScreen.WidgetLoadingScreen = WidgetPtr;

            // - Play Movies Setting
            LoadingScreen.bMoviesAreSkippable = true; //
            LoadingScreen.bWaitForManualStop = true; //
            LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Normal;
            LoadingScreen.MoviePaths.Add("NARRATIVA_placeholder");
            GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
        }
        else
        {
            isPlayingCinematic = false;
            FLoadingScreenAttributes LoadingScreen;
            LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;

            LoadingWidget = CreateWidget<UUserWidget>(this, WidgetTemplate);

            TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();
            LoadingScreen.WidgetLoadingScreen = WidgetPtr;

            GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

        }
    }
}

void USATORI_GameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
    if (!IsRunningDedicatedServer())
    {
        if (LoadingWidget)
        {
            FTimerHandle WaitHandle;

            GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
                {
					if(LoadingWidget)
					{
                        LoadingWidget->RemoveFromParent();
                        LoadingWidget->MarkPendingKill();
					}
                }), LoadingTimerValue, false);
        }
    }
}

void USATORI_GameInstance::SaveGameToDisk()
{
	
}

void USATORI_GameInstance::InitSaveGame()
{
	SaveGame = Cast<USATORI_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveGame)
	{
		SaveGame = Cast<USATORI_SaveGame>(UGameplayStatics::CreateSaveGameObject(USATORI_SaveGame::StaticClass()));
	}
}

void USATORI_GameInstance::RegisterActorToSave()
{

}

void USATORI_GameInstance::UnRegisterActorToSave()
{

}

void USATORI_GameInstance::FillPortalGrantedAbilityWithData()
{
    if (GrantedAbilityDataAsset)
    {
        for (const FPortalGrantedAbilityDatas& Data : GrantedAbilityDataAsset->GrantedAbilities)
        {
            FString CurrentName = Data.PortalGrantedAbilityName.ToString();
            FName LocalAbilityName = FName(*CurrentName);

            if (LocalAbilityName.IsValid() && Data.PortalGrantedAbility)
            {
                FSATORI_PortalAbilitiesDatasReward GrantedAbility;
                GrantedAbility.AbilitiyIcon = Data.PortalGrantedAbilitiyIcon;
                GrantedAbility.CurrentAbility = Data.PortalGrantedAbility;
                GrantedAbility.AbilityName = Data.PortalGrantedAbilityName;
                GrantedAbility.isUpgrated = Data.isUpgrated;
                if (GrantedAbility.isUpgrated)
                {
                    PortalGrantedUpgratedAbilityToApply.Add(GrantedAbility);
                    PortalRecicledUpgratedAbility.Add(GrantedAbility);
                }
                else
                {
                    PortalGrantedNormalAbilityToApply.Add(GrantedAbility);
                    PortalRecicledNormalAbility.Add(GrantedAbility);
                }
            }
        }
    }

   /* ShuffleArray(PortalGrantedNormalAbilityToApply);
    ShuffleArray(PortalGrantedUpgratedAbilityToApply);*/
}

void USATORI_GameInstance::RemoveElementonFromNormalAbilities(int Id)
{
    if (PortalGrantedNormalAbilityToApply.Num() > 0)
    {
        int Index = PortalGrantedNormalAbilityToApply.Num() - Id;
        if (Index < 0)
        {
            Index = 0;
        }
        if (PortalGrantedNormalAbilityToApply.Num() == 2 && Id == 1)
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Index].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(0);
        }
        else if (PortalGrantedNormalAbilityToApply.Num() == 2 && Id == 2)
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Index].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(1);
        }
        else
        {
            FString AbilityString = PortalGrantedNormalAbilityToApply[Index].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Normal Ability Name : %s "), *AbilityString);
            PortalGrantedNormalAbilityToApply.RemoveAt(Index);
        }
        //ShuffleArray(PortalGrantedNormalAbilityToApply);
    }
}

void USATORI_GameInstance::RemoveElementonFromUpgratedAbilities(int Id)
{
    if (PortalGrantedUpgratedAbilityToApply.Num() > 0)
    {
        int Index = PortalGrantedUpgratedAbilityToApply.Num() - Id;
        if (Index < 0)
        {
            Index = 0;
        }
        if (PortalGrantedUpgratedAbilityToApply.Num() == 2 && Id == 1)
        {
            FString AbilityString = PortalGrantedUpgratedAbilityToApply[Index].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Upgrated Ability Name : %s "), *AbilityString);
            PortalGrantedUpgratedAbilityToApply.RemoveAt(0);
        }
        else if (PortalGrantedUpgratedAbilityToApply.Num() == 2 && Id == 2)
        {
            FString AbilityString = PortalGrantedUpgratedAbilityToApply[Index].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Upgrated Ability Name : %s "), *AbilityString);
            PortalGrantedUpgratedAbilityToApply.RemoveAt(1);
        }
        else
        {
            FString AbilityString = PortalGrantedUpgratedAbilityToApply[Index].AbilityName.ToString();
            UE_LOG(LogTemp, Warning, TEXT(" Player Upgrated Ability Name : %s "), *AbilityString);
            PortalGrantedUpgratedAbilityToApply.RemoveAt(Index);
        }
        //ShuffleArray(PortalGrantedUpgratedAbilityToApply);
    }
}

void USATORI_GameInstance::ResetPortalRewardAbilities()
{
    // Clear the player Abilities
    NormalAbilities.Empty();
    UpgratedAbilities.Empty();

    // Clear the Array
    PortalGrantedUpgratedAbilityToApply.Empty();
    PortalGrantedNormalAbilityToApply.Empty();

    // Allocate the recicled Array elements to portal Array
    PortalGrantedUpgratedAbilityToApply = PortalRecicledUpgratedAbility;
    PortalGrantedNormalAbilityToApply = PortalRecicledNormalAbility;

   /* ShuffleArray(PortalGrantedNormalAbilityToApply);
    ShuffleArray(PortalGrantedUpgratedAbilityToApply);*/
}

void USATORI_GameInstance::ShuffleArray(TArray<FSATORI_PortalAbilitiesDatasReward>& myArray)
{
    if (myArray.Num() > 0)
    {
        int32 LastIndex = myArray.Num() - 1;
        for (int32 i = 0; i <= LastIndex; ++i)
        {
            int32 Index = FMath::RandRange(i, LastIndex);
            if (i != Index)
            {
                myArray.Swap(i, Index);
            }
        }
    }
}