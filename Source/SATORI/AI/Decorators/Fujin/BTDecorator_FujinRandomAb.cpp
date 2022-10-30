// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/Fujin//BTDecorator_FujinRandomAb.h"
#include "AIController.h"
#include "AI/Character/Fujin/SATORI_Fujin.h"



bool UBTDecorator_FujinRandomAb::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuccess = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (!bSuccess) return false;

	float Probability = 33.0f;

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController->GetPawn();

	ASATORI_Fujin* AI = Cast<ASATORI_Fujin>(Pawn);

	if (AI)
	{
		if(AI->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(("Boss.Fase.Final"))) && !raijin_muerto)
		{
			
			TArray<float> Probs = AI->GetArrayProbs(ComboRaijin);

			if(Probs.Num()> (ComboFujin -1))
			{
				Probability = Probs[ComboFujin - 1];

				int randomnum = rand() % 100 + 1;

				if (randomnum < Probability)
				{
				
					if(Probs[ComboFujin - 1] - 5 >=0)
					{
						for(int i = 0; i < Probs.Num();i++)
						{
							if(i == (ComboFujin - 1))
							{
								Probs[i] += -5;
							}
							else
							{
								Probs[i] += 2.5;
							}
						}
						
					}
					else if(Probs[ComboFujin - 1] > 0 )
					{
						float dif = Probs[ComboFujin - 1] / 2;

						for (int i = 0; i < Probs.Num(); i++)
						{
							if (i == (ComboFujin - 1))
							{
								Probs[i] = 0;
							}
							else
							{
								Probs[i] += dif;
							}
						}


					}
					return true;
				}
				else
				{
					return false;
				}

			}

		
		}

		// 25%
		int randomnum1 = rand() % 100 + 1;

		if (randomnum1 < Probability)
		{
			return true;
		}
	}


	return false;

}