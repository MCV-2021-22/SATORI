// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SATORI_BlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API USATORI_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
		static float ApplyGameplayEffectDamage(AActor* DamagedActor, float BaseDamage, 
			AActor* DamageCauser, TSubclassOf<class UGameplayEffect> DamageEffect);
};
