#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SATORI_InteractInterface.generated.h"

UINTERFACE(Blueprintable)
class USATORI_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

class ISATORI_InteractInterface
{
	GENERATED_BODY()
public:
	virtual void Interact(AActor* ActorInteracting) { }
	virtual void StartCanInteract(AActor* ActorInteracting) { }
	virtual void StopCanInteract(AActor* ActorInteracting) { }
};

DECLARE_LOG_CATEGORY_EXTERN(LogInteract, Log, All);