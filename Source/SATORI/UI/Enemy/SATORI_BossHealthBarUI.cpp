// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/SATORI_BossHealthBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"
#include "AI/Character/Raijin/SATORI_Raijin.h"


bool USATORI_BossHealthBarUI::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORI_Fujin::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		Fujin = Cast<ASATORI_Fujin>(Actor);
		Raijin = Cast<ASATORI_Raijin>(Actor);
	}

	if (Fujin && Raijin)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy UI TESTING !!!!!!!!!!!"));
	}

	return true;
}


void USATORI_BossHealthBarUI::SetHealthBarPercentage(float Fujin_HealthValue, float Raijin_HealthValue)
{
	// TODO
}

void USATORI_BossHealthBarUI::SetHealthTextBlock(FText Fujin_HealthName, FText Raijin_HealthName)
{

}