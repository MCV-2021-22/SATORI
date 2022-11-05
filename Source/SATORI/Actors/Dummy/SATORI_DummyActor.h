// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "SATORI_DummyActor.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UAnimMontage;

UCLASS()
class SATORI_API ASATORI_DummyActor : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASATORI_DummyActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* DummyCollision = nullptr;

	UFUNCTION(BlueprintCallable)
	void PlayImpactAnimations();

	void CheckImpactReceivedByPlayer(EComboState State);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LightAttack_1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LightAttack_2 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LightAttack_3 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HeavyAttack = false;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* AnimToPlay_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* AnimToPlay_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* AnimToPlay_3;

	// Animation Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AnimMontageToPlay_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AnimMontageToPlay_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AnimMontageToPlay_3;

	UFUNCTION(BlueprintCallable)
	void PlayAnimationWithDamage();

	UFUNCTION(BlueprintCallable)
	void PlayDamagedAnimation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDamageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDamageOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
