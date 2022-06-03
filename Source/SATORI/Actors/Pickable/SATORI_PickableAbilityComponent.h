//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GAS/SATORI_GameplayAbility.h"
#include "SATORI_PickableAbilityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SATORI_API USATORI_PickableAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USATORI_PickableAbilityComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray < TSubclassOf < USATORI_GameplayAbility > > Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray < TSubclassOf < UGameplayEffect > > GameplayEffects;

};
