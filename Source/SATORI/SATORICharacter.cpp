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
#include "SATORI/GAS/SATORI_AbilitySystemComponent.h"
#include "Character/Mask/SATORI_AbilityMask.h"
#include "Components/Player/SATORI_StatsComponent.h"
#include "Components/Player/SATORI_ComboSystemComponent.h"
#include "AnimNotify/State/SATORI_ANS_JumpSection.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/Character/SATORI_AICharacter.h"
#include "GAS/Attributes/SATORI_AttributeSet.h"
#include "GameplayFramework/SATORI_GameInstance.h"
#include "Character/SATORI_PlayerController.h"
#include "FunctionLibrary/SATORI_BlueprintLibrary.h"
#include "GAS/Effects/SATORI_ManaRecoverEffect.h"
//Cheat related include
#include "Kismet/GameplayStatics.h"
#include "Components/Player/SATORI_InteractComponent.h"
#include "SATORIGameMode.h"

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
	ComboSystemComponent = CreateDefaultSubobject<USATORI_ComboSystemComponent>("ComboSystemComponent");
	SATORIAbilityMaskComponent = CreateDefaultSubobject<USATORI_AbilityMask>("MaskComponent");
	StatsComponent = CreateDefaultSubobject<USATORI_StatsComponent>("StatsComponent");
	AbilitySystemComponent = CreateDefaultSubobject<USATORI_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	PlayerGameplayAbilityComponent = CreateDefaultSubobject<USATORI_GameplayAbilityComponent>(TEXT("SATORI_GameplayAbilityComponent"));
	TargetSystemComponent = CreateDefaultSubobject<USATORI_TargetSystemComponent>(TEXT("TargetSystemComponent"));
	InteractComponent = CreateDefaultSubobject<USATORI_InteractComponent>(TEXT("InteractComponent"));

	//Hand Component
	HandComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Hand"));
	HandComponent->SetupAttachment(GetMesh(), "Bip001-R-Hand");

	// Weapon Component
	SwordComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	AttackingCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sword Collision"));
	if (SwordComponent)
	{
		const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		SwordComponent->AttachToComponent(GetMesh(), AttachmentRules, "BoSocket");
		// Sphere Collision
		AttackingCollision->SetCapsuleSize(20.f, 60.f, true);
		AttackingCollision->SetCollisionProfileName("Pawn");
		AttackingCollision->SetGenerateOverlapEvents(false);
		AttackingCollision->AttachTo(SwordComponent);

		AttackingCollision->OnComponentBeginOverlap.AddDynamic(this, &ASATORICharacter::OnWeaponOverlapBegin);
		AttackingCollision->OnComponentEndOverlap.AddDynamic(this, &ASATORICharacter::OnWeaponOverlapEnd);
	}
}

void ASATORICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("On Possessed"));
	USATORI_GameInstance* GameInstanceRef = Cast<USATORI_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ASATORI_PlayerState* PS = GetPlayerState<ASATORI_PlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<USATORI_AbilitySystemComponent>(PS->GetAbilitySystemComponent());	

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		AttributeSetBase = PS->GetSatoriAttributeSet();
		
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));

		Tags.Add("PossessedBy.Player");

		InitializePassiveAttributes();
		ApplyDefaultAbilities();

		ManaRecoverGameplayEffect = USATORI_ManaRecoverEffect::StaticClass()->GetDefaultObject<USATORI_GameplayEffect>();

		ASATORI_PlayerController* SatoriPlayerController = Cast<ASATORI_PlayerController>(GetController());
		if (SatoriPlayerController)
		{
			SatoriPlayerController->CreateMainHUD();
		}

		// Test Mask Effect
		/*MaskType = SATORIMaskType::Aka;
		SATORIAbilityMaskComponent->GrantedMaskEffects(MaskType);*/
		// -------------------
		
		if(GameInstanceRef->PlayerStart)
		{
			SetHealth(GetMaxHealth());
			SetMana(GetMaxMana());
			GameInstanceRef->PlayerStart = false;
			StatsComponent->InitializeStatsAttributes(PS);
		}
		else
		{
			StatsComponent->InitializeStatsAttributesByInstance(PS, GameInstanceRef);
			SATORIAbilityMaskComponent->GrantedMaskEffects(GameInstanceRef->MaskType);
		}

		// Set Health to Max Health Value
	}

	if (Cast<APlayerController>(NewController) != nullptr) {
		//GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));
		AddGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player"));
		//AddGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.AI"));
	}
}

void ASATORICharacter::ApplyDefaultAbilities()
{
	if (!PlayerGameplayAbilityComponent->DefaultAbilities)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAbility for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Granting a GameplayAbility to an ASC adds it to the ASC's list of ActivatableAbilities allowing it to activate the GameplayAbility
	for (FSATORIGameplayAbilityInfo Ability : PlayerGameplayAbilityComponent->DefaultAbilities->Abilities)
	{
		// GameplayAbilitySpec exists on the ASC after a GameplayAbility is granted and defines the activatable GameplayAbility
		GrantAbilityToPlayer(FGameplayAbilitySpec(Ability.SATORIAbility, 1, static_cast<uint32>(Ability.AbilityKeys), this));

		// Adding Remove Abilities, use for death, need to remove all abilities
		RemovedgameplayAbilities.Add(Ability.SATORIAbility.Get());
	}
}

bool ASATORICharacter::DoRayCast()
{
	const FVector StartPosition = GetActorLocation();
	const FRotator StartRotation = GetActorRotation();
	const FVector EndPosition = StartPosition + (StartRotation.Vector() * 300.0f);

	UWorld* World = GetWorld();
	FHitResult HitResult;
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActor(RootComponent->GetOwner());

	FVector delta = EndPosition - StartPosition;
	TWeakObjectPtr<AActor> NewActor;

	bool bHit = false;
	for (int i = -5; i <= 5; i++)
	{
		FVector Axis = FVector::ZAxisVector;
		float rad = FMath::DegreesToRadians(i * 5);
		FQuat quaternion = FQuat(Axis, rad);
		FRotator rotator = FRotator(quaternion);
		FVector newDelta = rotator.RotateVector(delta);

		FVector newEndPos = newDelta + StartPosition;

		bool newHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartPosition,
			newEndPos,
			ECollisionChannel::ECC_Pawn,
			Params
		);

		::DrawDebugLine(World, StartPosition, newEndPos, newHit ? FColor::Green : FColor::Red, false, 1.0f);
		if (newHit)
		{
			NewActor = HitResult.Actor;
			bHit = true;
			break;
		}
	}

	if (bHit)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, HitResult.GetActor()->GetFName().ToString());
	}

	TWeakObjectPtr<ASATORI_AICharacter> AICharacter = Cast<ASATORI_AICharacter>(HitResult.Actor);
	if (AICharacter.IsValid())
	{
		bool isInFront = AICharacter->CheckPlayerWithRayCast();
		if (isInFront)
		{
			if (AICharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Lured"))))
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy"));

				AICharacter->AddGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned")));

				FGameplayEventData EventData;
				EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("State.Stunned.Start"));
				AICharacter->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("State.Stunned.Start")), &EventData);

				return true;
			}
			if (AICharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned"))))
			{
				return false;
			}
		}	
	}
	return false;
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


void ASATORICharacter::CharacterDeath()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	if (DeathMontage)
	{
		ASATORI_PlayerController* SatoriController = Cast<ASATORI_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (SatoriController)
		{
			DisableInput(SatoriController);
			SatoriController->SetShowMouseCursor(true);
			ShowDeathWidget();
		}
		PlayAnimMontage(DeathMontage);
	}
}

void ASATORICharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	if (RemovedgameplayAbilities.Num() > 0)
	{
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && RemovedgameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}
	}
	
	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
}

void ASATORICharacter::SetCharacterMask(SATORIMaskType GrantedMaskType)
{
	if (GrantedMaskType != SATORIMaskType::NONE)
	{
		MaskType = GrantedMaskType;
		SATORIAbilityMaskComponent->GrantedMaskEffects(MaskType);
	}
}

void ASATORICharacter::RemoveMaskGameplayEffect()
{
	AbilitySystemComponent->RemoveActiveGameplayEffect(SATORIAbilityMaskComponent->GetCurrentActiveGEHandle());

	//AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(SATORIAbilityMaskComponent->ChooseMaskEffectoToApply(MaskType),
	//	AbilitySystemComponent.Get());
}

void ASATORICharacter::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ASATORI_AICharacter* EnemyCharacter = Cast<ASATORI_AICharacter>(OtherActor);
		if (EnemyCharacter)
		{
			float Damage_Values = USATORI_BlueprintLibrary::ApplyGameplayEffectDamage(EnemyCharacter, 40, this, DamageEffect);
			AbilitySystemComponent->ApplyGameplayEffectToSelf(ManaRecoverGameplayEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());
			AttackingCollision->SetGenerateOverlapEvents(false);
			EnemyCharacter->sendDamage(Damage_Values);
			AnimactionPlayRater = 0.5f;
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), AnimactionPlayRater);
		}
	}
}

void ASATORICharacter::OnWeaponOverlapEnd(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		AnimactionPlayRater = 1.0f;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), AnimactionPlayRater);
	}
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

	// Interact Action
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASATORICharacter::OnInteract);

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

void ASATORICharacter::OnInteract()
{
	InteractComponent->TryToInteract();
}

void ASATORICharacter::SetComboJumpSection(USATORI_ANS_JumpSection* JumpSection)
{
	this->JumpSectionNS = JumpSection;

	if (this->JumpSectionNS != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set jump section %s"), *JumpSection->NextMontageNames[0].ToString());
	}
}

bool ASATORICharacter::AttackJumpSectionCombo()
{
	if (this->JumpSectionNS == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("JumpSection failed : No JumpSectioNS!"));
		return false;
	}

	if (!GetMesh())
	{
		return false;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerJumpSection failed: no anim instance!"));
		return false;
	}

	UAnimMontage* CurrentActiveMontage = AnimInstance->GetCurrentActiveMontage();
	if (!CurrentActiveMontage)
	{
		UE_LOG(LogTemp, Display, TEXT("TriggerJumpSection failed: no current montage!"));
		return false;
	}

	const FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(CurrentActiveMontage);

	const int RandInt = FMath::RandRange(0, this->JumpSectionNS->NextMontageNames.Num() - 1);
	const FName NextSectionName = JumpSectionNS->NextMontageNames[RandInt];

	AnimInstance->Montage_JumpToSection(NextSectionName, CurrentActiveMontage);

	return true;
}

bool ASATORICharacter::PlayerActiveAbilityWithTag(FGameplayTag TagName)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(TagName);

	if (!AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilitiesByTag failed "));
		return false;
	}

	return true;
}

//Cheats
void ASATORICharacter::SetGodMode()
{
	SetDefense(100);
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());

}

void ASATORICharacter::RestartStats()
{
	SetHealth(GetMaxHealth());
	SetMana(GetMaxMana());
}

void ASATORICharacter::GetAllAbilities()
{
	/*for (TSubclassOf < USATORI_GameplayAbility > Ability : PlayerGameplayAbilityComponent->DisabledAbilityClasses)
	{
		PlayerGameplayAbilityComponent->EnabledAbilityClasses.AddUnique(Ability);
	}
	PlayerGameplayAbilityComponent->DisabledAbilityClasses.Empty();*/
}

void ASATORICharacter::RemoveAllAbilities()
{
	for (const TPair<FName, FSATORI_AbilitiesDatas>& pair : PlayerGameplayAbilityComponent->PlayerGameplayAbility)
	{
		PlayerGameplayAbilityComponent->PlayerGameplayAbilityDissabled.Add(pair);
		PlayerGameplayAbilityComponent->PlayerAbilitiesNamesDissabled.Add(pair.Key);
	}
	PlayerGameplayAbilityComponent->PlayerGameplayAbility.Empty();
	PlayerGameplayAbilityComponent->PlayerAbilitiesNames.Empty();
	PlayerGameplayAbilityComponent->CurrentAbilityValue = 0;
	PlayerGameplayAbilityComponent->PrevAbilityValue = 0;
	PlayerGameplayAbilityComponent->NextAbilityValue = 0;
}

void ASATORICharacter::GetAbility(FName AbilityName)
{
	/*TSubclassOf < USATORI_GameplayAbility > AbilityToEnable;

	for (TSubclassOf < USATORI_GameplayAbility > Ability : PlayerGameplayAbilityComponent->DisabledAbilityClasses)
	{
		FName GetAbilityName = Ability.GetDefaultObject()->GetAbilityName();
		UE_LOG(LogTemp, Display, TEXT("GetAbilityName: %s"), *GetAbilityName.ToString());
		UE_LOG(LogTemp, Display, TEXT("AbilityName: %s"), *AbilityName.ToString());
		if (GetAbilityName.ToString() == AbilityName.ToString())
		{
			AbilityToEnable = Ability;
			PlayerGameplayAbilityComponent->EnabledAbilityClasses.AddUnique(AbilityToEnable);
		}
	}
	PlayerGameplayAbilityComponent->DisabledAbilityClasses.Remove(AbilityToEnable);*/
}

void ASATORICharacter::GetEnabledAbilityName()
{
	/*for (TSubclassOf < USATORI_GameplayAbility > Ability : PlayerGameplayAbilityComponent->EnabledAbilityClasses)
	{
		FName GetAbilityName = Ability.GetDefaultObject()->GetAbilityName();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("AbilityName: %s"), *GetAbilityName.ToString()));
	}*/
}

void ASATORICharacter::KillAllEnemies()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASATORI_AICharacter::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		GetWorld()->GetAuthGameMode<ASATORIGameMode>()->RemoveEnemyActor(Actor);
		Actor->Destroy();
	}
}

void ASATORICharacter::KillPlayer()
{
	SetHealth(0);
}