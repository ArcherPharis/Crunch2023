// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_GroundBlastBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GATargetActor_GroundPick.h"
#include "GameFrameWork/Character.h"
#include "CTCharacterBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include  "AbilitySystemComponent.h"
void UGA_GroundBlastBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CheckAbilityCooldown() || !K2_CheckAbilityCost())
	{
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CastTargetingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, TargetingMontage);
	if (CastTargetingMontageTask)
	{
		CastTargetingMontageTask->OnBlendOut.AddDynamic(this, &UGA_GroundBlastBase::TargetingMontageOut);
		CastTargetingMontageTask->OnCancelled.AddDynamic(this, &UGA_GroundBlastBase::TargetingMontageOut);
		CastTargetingMontageTask->OnInterrupted.AddDynamic(this, &UGA_GroundBlastBase::TargetingMontageOut);
		CastTargetingMontageTask->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* WaitTargetStart = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, StartTargetingTag);
	if (WaitTargetStart)
	{
		WaitTargetStart->EventReceived.AddDynamic(this, &UGA_GroundBlastBase::StartTargeting);
		WaitTargetStart->ReadyForActivation();
	}
}

void UGA_GroundBlastBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACTCharacterBase* CTCharacterBase = GetAvatarAsCTCharacter();
	if (CTCharacterBase)
	{
		CTCharacterBase->SetEanbleAiming(false);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_GroundBlastBase::StartTargeting(FGameplayEventData Payload)
{
	ACTCharacterBase* CTCharacterBase = GetAvatarAsCTCharacter();
	if (CTCharacterBase)
	{
		CTCharacterBase->SetEanbleAiming(true);
	}

	UAbilityTask_WaitTargetData* WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, TargetActorClass);
	WaitTargetData->ValidData.AddDynamic(this, &UGA_GroundBlastBase::TargetAquired);
	WaitTargetData->Cancelled.AddDynamic(this, &UGA_GroundBlastBase::TargetCancelled);
	WaitTargetData->ReadyForActivation();

	AGameplayAbilityTargetActor* TargetActor;
	WaitTargetData->BeginSpawningActor(this, TargetActorClass, TargetActor);
	WaitTargetData->FinishSpawningActor(this, TargetActor);
}

void UGA_GroundBlastBase::TargetingMontageOut()
{
	K2_EndAbility();
}

void UGA_GroundBlastBase::CastingAnimFinished()
{
	K2_EndAbility();
}

void UGA_GroundBlastBase::TargetAquired(const FGameplayAbilityTargetDataHandle& Data)
{
	CastTargetingMontageTask->EndTask();
	K2_CommitAbility();
	UAbilityTask_PlayMontageAndWait* CastingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CastingMontage);
	if (CastingMontageTask)
	{
		CastingMontageTask->OnBlendOut.AddDynamic(this, &UGA_GroundBlastBase::CastingAnimFinished);
		CastingMontageTask->OnCancelled.AddDynamic(this, &UGA_GroundBlastBase::CastingAnimFinished);
		CastingMontageTask->OnInterrupted.AddDynamic(this, &UGA_GroundBlastBase::CastingAnimFinished);
		CastingMontageTask->OnCompleted.AddDynamic(this, &UGA_GroundBlastBase::CastingAnimFinished);
		CastingMontageTask->ReadyForActivation();
	}

	TArray<AActor*> targetActors = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(Data, 1);
	for (auto targetActor : targetActors)
	{
		LaunchActorAsCharacter(targetActor,	BlastLaunchSpeed);
	}
	
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(LauchBlastEffect), Data);
	ApplyStunEffectToTarget(Data);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	FGameplayCueParameters gameplayCueParam;
	gameplayCueParam.Location = Data.Get(0)->GetEndPoint();
	ASC->AddGameplayCue(BlastGameplayCueTag, gameplayCueParam);
}

void UGA_GroundBlastBase::TargetCancelled(const FGameplayAbilityTargetDataHandle& Data)
{
	K2_EndAbility();
}

void UGA_GroundBlastBase::LaunchActorAsCharacter(AActor* Actor, float speed)
{
	ACharacter* actorAsCharacter = Cast<ACharacter>(Actor);
	if (actorAsCharacter)
	{
		actorAsCharacter->LaunchCharacter(FVector::UpVector * speed, true, true);
	}
}
