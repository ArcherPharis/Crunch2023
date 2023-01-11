// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCharacterBase.h"
#include "CTAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CTAttributeSet.h"
#include "HitDetectionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ACTCharacterBase::ACTCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<UCTAbilitySystemComponent>("AbilitySystemComp");
	AttributeSet = CreateDefaultSubobject<UCTAttributeSet>("AttributeSet");
	HitDetectionComp = CreateDefaultSubobject<UHitDetectionComponent>("HitDetectionComp");
	HitDetectionComp->SetupAttachment(GetMesh());

	PerceptionStimuliComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("PerceptionStimuliComp");
}

void ACTCharacterBase::ApplayInitialEffect()
{
	AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACTCharacterBase::HealthChanged);
	ApplyEffectToSelf(InitialEffect);
}

void ACTCharacterBase::ApplyRegenEffect()
{
	for (auto& RegenEffect : RegenEffects)
	{
		ApplyEffectToSelf(RegenEffect);
	}
}

void ACTCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComp->InitAbilityActorInfo(this, this);
}

void ACTCharacterBase::SetEanbleAiming(bool bEnableMoving)
{
	bIsAiming = bEnableMoving;
	if (bIsAiming)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
}

// Called when the game starts or when spawned
void ACTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	ApplayInitialEffect();
	ApplyRegenEffect();
	GiveAbility(BasicAttackAbility, -1, false);
	for (auto& abilityEntry : InitialAbilities)
	{
		GiveAbility(abilityEntry.Value, (int)abilityEntry.Key);
	}
	AbilitySystemComp->RegisterGameplayTagEvent(StunTag).AddUObject(this, &ACTCharacterBase::StunTagChanged);
}

void ACTCharacterBase::ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply)
{
	FGameplayEffectSpecHandle Spec = AbilitySystemComp->MakeOutgoingSpec(effectToApply, -1, AbilitySystemComp->MakeEffectContext());
	AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*Spec.Data);
}

void ACTCharacterBase::BasicAttack()
{
	 FGameplayAbilitySpec* MeleeAbilitySpec = AbilitySystemComp->FindAbilitySpecFromClass(BasicAttackAbility);
	 if (MeleeAbilitySpec->IsActive())
	 {
		 UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, BasicAttackCombo, FGameplayEventData());
	 }
	 else
	 {
		 AbilitySystemComp->TryActivateAbilityByClass(BasicAttackAbility);
	 }
}

UAbilitySystemComponent* ACTCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

FGameplayAbilitySpec* ACTCharacterBase::GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID, bool broadCast)
{
	FGameplayAbilitySpecHandle specHandle = AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(newAbility, -1, inputID));

	FGameplayAbilitySpec* spec = AbilitySystemComp->FindAbilitySpecFromHandle(specHandle);
	if (spec && broadCast)
	{
		OnAbilityGiven.Broadcast(spec);
	}
	return spec;
}

void ACTCharacterBase::HealthChanged(const FOnAttributeChangeData& AttributeData)
{
	BP_HealthUpdated(AttributeData.NewValue, AttributeData.NewValue - AttributeData.OldValue, AttributeSet->GetMaxHealth());
	
	if (AttributeData.NewValue <= 0)
	{
		StartDeathSequence();
	}

	if (AttributeData.NewValue < AttributeData.OldValue && !bIsDead && AttributeData.NewValue != AttributeData.OldValue)
	{
		OnCharacterTookDamage.Broadcast();
	}
}

void ACTCharacterBase::PauseAILogic(const FString& Reason)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		auto BrainComp = AIC->GetBrainComponent();
		if (BrainComp)
		{
			BrainComp->PauseLogic(Reason);
		}
	}
}

void ACTCharacterBase::ResumeAILogic(const FString& Reason)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		auto BrainComp = AIC->GetBrainComponent();
		if (BrainComp)
		{
			BrainComp->ResumeLogic(Reason);
		}
	}
}

void ACTCharacterBase::DisablePlayerInput()
{
	APlayerController* pc = GetController<APlayerController>();
	if (pc)
	{
		DisableInput(pc);
	}
}

void ACTCharacterBase::EnablePlayerInput()
{
	APlayerController* pc = GetController<APlayerController>();
	if (pc)
	{
		EnableInput(pc);
	}
}

void ACTCharacterBase::Disable()
{
	PauseAILogic();
	DisablePlayerInput();
}

void ACTCharacterBase::RecoverFromDisable()
{
	if (bIsDead) return;

	ResumeAILogic();
	EnablePlayerInput();
}

void ACTCharacterBase::StunTagChanged(const FGameplayTag tag, int32 count)
{
	if (count > 0)
	{
		Disable();
	}
	else
	{
		RecoverFromDisable();
	}
}


TSubclassOf<UGameplayAbility> ACTCharacterBase::GetAbilityByIndex(int index) const
{
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	InitialAbilities.GenerateValueArray(Abilities);
	if (Abilities.Num() > index)
	{
		return Abilities[index];
	}

	return TSubclassOf<UGameplayAbility>(nullptr);
}

void ACTCharacterBase::DoBasicAttack()
{
	BasicAttack();
}

void ACTCharacterBase::StartDeathSequence()
{
	if (bIsDead) return;

	bIsDead = true;
	OnCharacterDeathStarted.Broadcast();
	ApplyEffectToSelf(DeathEffect);
	StopAllAbilitites();
	AbilitySystemComp->SetActive(false);
	Disable();
	DisableAIRelavency();
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();	
	if (AnimInst && DeathMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dead, Playing death montage"))
		float DeathAnimDuration = AnimInst->Montage_Play(DeathMontage);
		FTimerHandle DeathTimerHandle;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACTCharacterBase::OnDeath, DeathAnimDuration + DeathDelayAfterMontage, false);
	}
	else
	{
		OnDeath();
	}
}

void ACTCharacterBase::OnDeath()
{
	BP_OnDeath();
	OnCharacterDeath.Broadcast();
	Destroy();
}

void ACTCharacterBase::StopAllAbilitites()
{
	AbilitySystemComp->CancelAllAbilities();
}

void ACTCharacterBase::DisableAIRelavency()
{
	PerceptionStimuliComp->UnregisterFromSense(UAISense_Sight::StaticClass());
	PerceptionStimuliComp->UnregisterFromPerceptionSystem();
}
