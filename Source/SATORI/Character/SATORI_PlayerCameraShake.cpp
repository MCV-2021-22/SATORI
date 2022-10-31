// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SATORI_PlayerCameraShake.h"

USATORI_PlayerCameraShake::USATORI_PlayerCameraShake()
{
	OscillationDuration = Local_OscillationDuration;
	OscillationBlendInTime = Local_OscillationBlendInTime;
	OscillationBlendOutTime = Local_OscillationBlendOutTime;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(Pitch_Amplitude, Pitch_Max_Amplitude);
	RotOscillation.Pitch.Frequency = FMath::RandRange(Pitch_Frequency, Pitch_Max_Frequency);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(Yaw_Amplitude, Yaw_Max_Amplitude);
	RotOscillation.Yaw.Frequency = FMath::RandRange(Yaw_Frequency, Yaw_Max_Frequency);

	RotOscillation.Roll.Amplitude = FMath::RandRange(Roll_Amplitude, Roll_Max_Amplitude);
	RotOscillation.Roll.Frequency = FMath::RandRange(Roll_Frequency, Roll_Max_Frequency);
}