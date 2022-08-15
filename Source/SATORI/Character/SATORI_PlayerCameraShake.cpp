// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SATORI_PlayerCameraShake.h"

USATORI_PlayerCameraShake::USATORI_PlayerCameraShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(0.1f, 0.5f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(1.0f, 5.f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.1f, 0.5f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(1.0f, 5.f);
}