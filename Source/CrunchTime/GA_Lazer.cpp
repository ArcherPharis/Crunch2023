// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Lazer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UGA_Lazer::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false, false))
		return;

	if (!CheckCost(Handle, ActorInfo))
		return;
	GetAbilitySystemComponentFromActorInfo()->NotifyAbilityCommit(this);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CostEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, MakeOutgoingGameplayEffectSpec(CostGameplayEffectClass));
	UAbilityTask_PlayMontageAndWait* PlayCastingMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CastingMontage);
	if (PlayCastingMontageAndWait)
	{
		PlayCastingMontageAndWait->OnBlendOut.AddDynamic(this, &UGA_Lazer::CastingEnded);
		PlayCastingMontageAndWait->OnCancelled.AddDynamic(this, &UGA_Lazer::CastingEnded);
		PlayCastingMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Lazer::CastingEnded);
		PlayCastingMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_Lazer::CastingEnded);
		PlayCastingMontageAndWait->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* WaitStartCastingLazerTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, StartLazerTag);
	if (WaitStartCastingLazerTask)
	{
		WaitStartCastingLazerTask->EventReceived.AddDynamic(this, &UGA_Lazer::StartCastingLazer);
		WaitStartCastingLazerTask->ReadyForActivation();
	}
	SetEnableAim(true);
}

void UGA_Lazer::CastingEnded()
{ 
	SetEnableAim(false);
	BP_RemoveGameplayEffectFromOwnerWithHandle(CostEffectHandle, -1);
	K2_EndAbility();
}

void UGA_Lazer::TargetAquired(const FGameplayAbilityTargetDataHandle& Data)
{

	if (!K2_CheckAbilityCost())
	{
		CastingEnded();
		return;
	}

	PushActorsInTargetData(Data, GetAvatarActorFromActorInfo()->GetActorForwardVector(), lazerPushSpeed);
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(DamageEffect), Data);
	ApplyStunEffectToTarget(Data);
}

void UGA_Lazer::TargetingCancelled()
{
	CastingEnded();
}

void UGA_Lazer::StartCastingLazer(FGameplayEventData Payload)
{
	UAbilityTask_WaitTargetData* WaitLazerTarget = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::CustomMulti, LazerTargetingActorClass);
	if (WaitLazerTarget)
	{
		WaitLazerTarget->ValidData.AddDynamic(this, &UGA_Lazer::TargetAquired);
		WaitLazerTarget->ReadyForActivation();

		AGameplayAbilityTargetActor* TargetActor;
		WaitLazerTarget->BeginSpawningActor(this, LazerTargetingActorClass, TargetActor);
		
		TargetActor->OwningAbility = this;
		AActor* avatarActor = GetAvatarActorFromActorInfo();
		TargetActor->PostSpawnInitialize(avatarActor->GetActorTransform(), avatarActor, nullptr, false, true, true);
		
		WaitLazerTarget->FinishSpawningActor(this, TargetActor);
		
		//TargetActor->AttachToActor(avatarActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		USkeletalMeshComponent* CasterSMC = GetOwningComponentFromActorInfo();
		if (CasterSMC)
		{
			TargetActor->AttachToComponent(CasterSMC, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSlotName);
		}
	}

	GetAbilitySystemComponentFromActorInfo()->GenericLocalCancelCallbacks.AddDynamic(this, &UGA_Lazer::TargetingCancelled);
}
