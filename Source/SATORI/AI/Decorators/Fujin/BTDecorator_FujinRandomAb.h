// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_FujinRandomAb.generated.h"


UCLASS()
class SATORI_API UBTDecorator_FujinRandomAb : public UBTDecorator
{
	GENERATED_BODY()

public:

	
	
	

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int ComboFujin = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int ComboRaijin = 0;

	UPROPERTY(EditAnywhere)
		bool raijin_muerto = false;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
