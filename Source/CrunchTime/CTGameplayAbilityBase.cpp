// Fill out your copyright notice in the Description page of Project Settings.


#include "CTGameplayAbilityBase.h"
#include "CTAttributeSet.h"
#include "CTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"

void UCTGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//this actually commit the ability, don't call if not wanting it.
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		if (TriggerEventData)
		{
			// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
			K2_ActivateAbilityFromEvent(*TriggerEventData);
		}
		else
		{
			UE_LOG(LogAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use Activate Ability instead of Activate Ability From Event."), *GetName());
			bool bReplicateEndAbility = false;
			bool bWasCancelled = true;
			EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		}
	}

	AvatarCTCharacterBase = Cast<ACTCharacterBase>(ActorInfo->AvatarActor);
}

bool UCTGameplayAbilityBase::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	bool commitSuccess = Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
	return commitSuccess;
}

ACTCharacterBase* UCTGameplayAbilityBase::GetAvatarAsCTCharacter() const
{
	if (AvatarCTCharacterBase == nullptr)
	{
		return Cast<ACTCharacterBase>(GetAvatarActorFromActorInfo());
	}
	return AvatarCTCharacterBase;
}

void UCTGameplayAbilityBase::SetEnableAim(bool bAimMode)
{
	ACTCharacterBase* characterBase = GetAvatarAsCTCharacter();
	if (!characterBase)
	{
		return;
	}

	characterBase->SetEanbleAiming(bAimMode);
}

void UCTGameplayAbilityBase::PushActorAsCharacter(AActor* actorToPush, const FVector& PushDir, float PushStrength) const
{
	ACharacter* actorAsCharacter = Cast<ACharacter>(actorToPush);
	if (actorAsCharacter)
	{
		actorAsCharacter->LaunchCharacter(PushDir.GetSafeNormal() * PushStrength, true, true);
	}
}

void UCTGameplayAbilityBase::PushActorsInTargetData(const FGameplayAbilityTargetDataHandle& tagetData, const FVector& PushDir, float PushStrength) const
{
	TArray<AActor*> actors = UAbilitySystemBlueprintLibrary::GetAllActorsFromTargetData(tagetData);
	for (AActor* actor : actors)
	{
		PushActorAsCharacter(actor, PushDir, PushStrength);
	}
}

float UCTGameplayAbilityBase::GetCooldownDuration() const
{
	float duration = 0;
	GetCooldownGameplayEffect()->DurationMagnitude.GetStaticMagnitudeIfPossible(-1, duration);
	return duration;
}

float UCTGameplayAbilityBase::GetStaimiaCost() const
{
	if (!GetCostGameplayEffect())
	{
		return 0;
	}

	float cost = 0;

	for (const FGameplayModifierInfo& modifierInfo : GetCostGameplayEffect()->Modifiers)
	{
		if (modifierInfo.Attribute == UCTAttributeSet::GetStaminaAttribute())
		{
			modifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(), cost);
		}
	}
	return -cost;
}

int UCTGameplayAbilityBase::GetAssignedLevel(UAbilitySystemComponent* owningComp) const
{

	int level = 1;
	if (owningComp)
	{
		FGameplayAbilitySpec* spec = owningComp->FindAbilitySpecFromClass(GetClass());
		level = GetAbilityLevel(spec->Handle, owningComp->AbilityActorInfo.Get());
	}

	return level;
}

void UCTGameplayAbilityBase::ApplyStunEffectToTarget(FGameplayAbilityTargetDataHandle TargetData)
{
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(StunEffect), TargetData);
}

bool UCTGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (GetAbilityLevel(Handle, ActorInfo) < 1)
	{
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

bool UCTGameplayAbilityBase::IsOtherHostile(const AActor* otherActor) const
{
	IGenericTeamAgentInterface* OwnerAsTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());
	if (OwnerAsTeamInterface && otherActor && OwnerAsTeamInterface->GetTeamAttitudeTowards(*otherActor) == ETeamAttitude::Hostile)
	{
		return true;
	}
	return false;
}