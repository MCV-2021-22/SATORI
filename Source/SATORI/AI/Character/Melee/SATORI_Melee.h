//

#pragma once

#include "CoreMinimal.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "FMODBlueprintStatics.h"
#include "SATORI_Melee.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UMaterialInstanceDynamic;

UCLASS()
class SATORI_API ASATORI_Melee : public ASATORI_AICharacter
{
	GENERATED_BODY()

public:

	ASATORI_Melee();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponentRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SwordComponentLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCapsuleComponent* AttackingCollisionLeft;

	// Weapon Overlap
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float WeaponDamage = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	FGameplayTag HeavyHitTag;

	void WeaponDissolveAfterDeath();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	UFMODEvent* MeleeAttack = nullptr;

protected:
	virtual void BeginPlay() override;

	// Material
	TArray<UMaterialInstanceDynamic*> Left_WeaponDynamicMaterials;
	TArray<UMaterialInstanceDynamic*> Right_WeaponDynamicMaterials;

	float Left_Weapon_TimeCountDown = 1.0f;
	float Right_Weapon_TimeCountDown = 1.0f;
	float LocalRate = 0.1f;
	FTimerHandle Left_MaterialWaitHandle;
	FTimerHandle Right_MaterialWaitHandle;
};
