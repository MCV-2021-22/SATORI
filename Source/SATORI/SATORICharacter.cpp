// Copyright Epic Games, Inc. All Rights Reserved.

#include "SATORICharacter.h"
#include "SATORI.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "SATORI/Data/SATORI_AbilityDataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "Character/SATORI_PlayerState.h"
#include "SATORI/GAS/Attributes/SATORI_AttributeSet.h"
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Character/Mask/SATORI_AbilityMask.h"

//////////////////////////////////////////////////////////////////////////
// ASATORICharacter

ASATORICharacter::ASATORICharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Components
	SATORIAbilityMaskComponent = CreateDefaultSubobject<USATORI_AbilityMask>("MaskComponent");
	/*AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);*/
}

void ASATORICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("On Possessed"));

	ASATORI_PlayerState* PS = GetPlayerState<ASATORI_PlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<USATORI_AbilitySystemComponent>(PS->GetAbilitySystemComponent());	

		AttributeSetBase = PS->GetSatoriAttributeSet();

		InitializePassiveAttributes();
		ApplyDefaultAbilities();

		// Test Mask Effect
		MaskType = SATORIMaskType::Aka;
		SATORIAbilityMaskComponent->GrantedMaskEffects(MaskType);
		// -------------------

		// Set Health to Max Health Value
		SetHealth(GetMaxHealth());
	}		
}

UAbilitySystemComponent* ASATORICharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASATORICharacter::ApplyDefaultAbilities()
{
	if (!DefaultAbilities)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAbility for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	for (FSATORIGameplayAbilityInfo Ability : DefaultAbilities->Abilities)
	{
		GrantAbilityToPlayer(FGameplayAbilitySpec(Ability.SATORIAbility, 1, static_cast<uint32>(Ability.AbilityKeys), this));
	}
}

void ASATORICharacter::GrantAbilityToPlayer(FGameplayAbilitySpec Ability)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!Ability.Ability)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s() Ability Not Granted for %s. Ability is not valid."), *FString(__FUNCTION__), *GetName());
		return;
	}

	AbilitySystemComponent->GiveAbility(Ability);
}

void ASATORICharacter::InitializePassiveAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Now apply passives
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),
				AbilitySystemComponent.Get());
		}
	}
}

// Getters
float ASATORICharacter::GetHealth() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetHealth();

	return 0.0f;
}

float ASATORICharacter::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetMaxHealth();

	return 0.0f;
}

float ASATORICharacter::GetDefense() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetDefense();

	return 0.0f;
}

float ASATORICharacter::GetAttack() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetAttack();

	return 0.0f;
}

float ASATORICharacter::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetMoveSpeed();

	return 0.0f;
}

float ASATORICharacter::GetGold() const
{
	if (AttributeSetBase.IsValid())
		return AttributeSetBase->GetGold();

	return 0.0f;
}

int32 ASATORICharacter::GetCharacterLevel() const
{
	return 1;
}

// Setters
void ASATORICharacter::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
		AttributeSetBase->SetHealth(Health);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASATORICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASATORICharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASATORICharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASATORICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASATORICharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASATORICharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASATORICharacter::TouchStopped);

	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
		FString("CancelTarget"), FString("ESATORIAbilityInputID"), static_cast<int32>(ESATORIAbilityInputID::Type::Confirm), static_cast<int32>(ESATORIAbilityInputID::Type::Cancel)));
}

void ASATORICharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASATORICharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASATORICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASATORICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASATORICharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASATORICharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
