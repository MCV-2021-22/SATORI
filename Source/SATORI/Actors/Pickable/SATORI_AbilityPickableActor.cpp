//

#include "Actors/Pickable/SATORI_AbilityPickableActor.h"
#include "Components/BoxComponent.h"
#include "Components/Player/SATORI_GameplayAbilityComponent.h"
#include "SATORICharacter.h"

ASATORI_AbilityPickableActor::ASATORI_AbilityPickableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(FName("Trigger"));

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASATORI_AbilityPickableActor::OnComponentBeginOverlap);

	AbilityComponent = CreateDefaultSubobject<USATORI_PickableAbilityComponent>(TEXT("AbilityArrayComponent"));

}

void ASATORI_AbilityPickableActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ASATORICharacter* Player = Cast<ASATORICharacter>(OtherActor);

	if(!Player)
	{
		return;
	}

	if (AbilityComponent->Abilities.Num() != 0)
	{
		int RandomAbility = FMath::RandRange(0, AbilityComponent->Abilities.Num() - 1);
		TSubclassOf < USATORI_GameplayAbility > AbilityToGive = AbilityComponent->Abilities[RandomAbility];
		Player->PlayerGameplayAbilityComponent->AddEnabledAbilityClass(AbilityToGive);
		AbilityComponent->Abilities.Remove(AbilityToGive);
	}

	if (AbilityComponent->GameplayEffects.Num() != 0)
	{

		int RandomEffect = FMath::RandRange(0, AbilityComponent->GameplayEffects.Num() - 1);
		TSubclassOf < UGameplayEffect > EffectToGive = AbilityComponent->GameplayEffects[RandomEffect];
		FGameplayEffectContextHandle GameplayEffectContext;
		UAbilitySystemComponent* Stats = Player->GetAbilitySystemComponent();
		FGameplayEffectContextHandle ContextHandle;
		FGameplayEffectSpecHandle SpecHandle = Stats->MakeOutgoingSpec(EffectToGive, 1.0f, ContextHandle);
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
		Stats->ApplyGameplayEffectSpecToSelf(*Spec);

	}
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}