// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "SATORI_PlayerCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_PlayerCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()
	
public:
	USATORI_PlayerCameraShake();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Local_OscillationDuration = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Local_OscillationBlendInTime = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Local_OscillationBlendOutTime = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Pitch_Amplitude = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Pitch_Frequency = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Pitch_Max_Amplitude = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Pitch_Max_Frequency = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Yaw_Amplitude = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Yaw_Frequency = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Yaw_Max_Amplitude = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Yaw_Max_Frequency = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Roll_Amplitude = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Roll_Frequency = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Roll_Max_Amplitude = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float Roll_Max_Frequency = 5.0f;
};
