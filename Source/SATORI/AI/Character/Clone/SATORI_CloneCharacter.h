//

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "SATORI_CloneCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UGameplayEffect;

UCLASS()
class SATORI_API ASATORI_CloneCharacter : public ASATORI_AICharacter
{
	GENERATED_BODY()
	
public:

	ASATORI_CloneCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponentClone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponDamage = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bMultipleHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gamepaly Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gamepaly Effect")
	TSubclassOf<UGameplayEffect> BlockCountGameplayEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gamepaly Effect")
	TSubclassOf<UGameplayEffect> StunGameplayEffect;

	// Weapon Overlap
	UFUNCTION()
	void OnWeaponOverlapBegin(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

protected:

	virtual void BeginPlay() override;

};
