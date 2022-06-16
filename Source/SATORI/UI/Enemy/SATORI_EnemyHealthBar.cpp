// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/SATORI_EnemyHealthBar.h"
#include "Components/ProgressBar.h"

bool USATORI_EnemyHealthBar::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	return true;
}

void USATORI_EnemyHealthBar::UpdateHealthBar(float health, float maxHealth)
{
	float value = health / maxHealth;
	if (HealthBar)
	{
		HealthBar->SetPercent(value);
	}
}