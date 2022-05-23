//

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_CloneCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SATORI_API ASATORI_CloneCharacter : public ASATORI_AICharacter
{
	GENERATED_BODY()
	
public:

	ASATORI_CloneCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Clone")
	float SphereRadiusOfAction = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Clone")
	float TimeToDestroy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true), Category = "Clone")
	FName TagGrantedWhenLured;

};
