// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SATORI_PlayerController.generated.h"

class USATORI_MainUI;

UCLASS()
class SATORI_API ASATORI_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASATORI_PlayerController(const class FObjectInitializer& InitializerObject);

	USATORI_MainUI* GetSatoriMainUI() { return SATORIMainWidget; }

protected:
	virtual void OnPossess(APawn* aPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USATORI_MainUI> SATORIMainUI;

	class USATORI_MainUI* SATORIMainWidget;
};
