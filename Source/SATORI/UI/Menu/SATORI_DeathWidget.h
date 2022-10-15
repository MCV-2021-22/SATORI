// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SATORI_DeathWidget.generated.h"

class UButton;

UCLASS()
class SATORI_API USATORI_DeathWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LobbyMenus = "ThirdPersonExampleMap";

	UPROPERTY(meta = (BindWidget))
	UButton* DeathButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;

protected:
	UFUNCTION()
	void OnDeathButtonClicked();
	UFUNCTION()
	void OnMainMenuClicked();
	UFUNCTION()
	void OnRetryClicked();
};
