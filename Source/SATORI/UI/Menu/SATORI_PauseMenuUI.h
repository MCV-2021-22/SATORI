// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_PauseMenuUI.generated.h"

class UButton;

UCLASS()
class SATORI_API USATORI_PauseMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MainMenuLevelName = "MainMenu";

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;

private:
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnMainMenuClicked();
};
