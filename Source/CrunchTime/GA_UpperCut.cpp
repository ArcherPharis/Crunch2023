// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_UpperCut.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirm.h"
#include "CTAbilitySystemBlueprintLibrary.h"
#include "GATargetActor_Frontal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGA_UpperCut::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* playUpperCutMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, UpperCutMontage);
	if (playUpperCutMontageAndWait)
	{
		playUpperCutMontageAndWait->OnBlendOut.AddDynamic(this, &UGA_UpperCut::MontageFinished);
		playUpperCutMontageAndWait->OnCancelled.AddDynamic(this, &UGA_UpperCut::MontageFinished);
		playUpperCutMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_UpperCut::MontageFinished);
		playUpperCutMontageAndWait->OnCompleted.AddDynamic(this, &UGA_UpperCut::MontageFinished);
		playUpperCutMontageAndWait->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* waitCutStart = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CutStartEventTag);
	if (waitCutStart)
	{
		waitCutStart->EventReceived.AddDynamic(this, &UGA_UpperCut::UpperCutStart);
		waitCutStart->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* WaitComboUpdate = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboEventTag);
	if (WaitComboUpdate)
	{
		WaitComboUpdate->EventReceived.AddDynamic(this, &UGA_UpperCut::UpdateComb);
		WaitComboUpdate->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* WaitComboHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboHitEventTag);
	if (WaitComboHit)
	{
		WaitComboHit->EventReceived.AddDynamic(this, &UGA_UpperCut::ComboHit);
		WaitComboHit->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* WaitFinalBlow = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboFinalBlowEventTag);
	if (WaitFinalBlow)
	{
		WaitFinalBlow->EventReceived.AddDynamic(this, &UGA_UpperCut::FinalBlow);
		WaitFinalBlow->ReadyForActivation();
	}
}

void UGA_UpperCut::MontageFinished()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UGA_UpperCut::UpperCutStart(FGameplayEventData Payload)
{
	UAbilityTask_WaitTargetData* waitFrontalTargetData = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::Instant, targetActorClass);
	if (waitFrontalTargetData)
	{
		waitFrontalTargetData->ValidData.AddDynamic(this, &UGA_UpperCut::LaunchTargets);
		waitFrontalTargetData->ReadyForActivation();
		AGameplayAbilityTargetActor* targetActor;
		waitFrontalTargetData->BeginSpawningActor(this, targetActorClass, targetActor);
		AActor* Avatar = GetAvatarActorFromActorInfo();
		targetActor->PostSpawnInitialize(Avatar->GetActorTransform(), Avatar, (APawn*)Avatar, false, true, true);
		waitFrontalTargetData->FinishSpawningActor(this, targetActor);
	}
}

void UGA_UpperCut::UpdateComb(FGameplayEventData Payload)
{
	NextComboSectionName = UCTAbilitySystemBlueprintLibrary::GetRandomNameFromTagContainer(Payload.TargetTags);

	UAbilityTask_WaitTargetData* waitCombo = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, targetActorClass);
	if (waitCombo)
	{
		waitCombo->ValidData.AddDynamic(this, &UGA_UpperCut::TryActivateCombo);
		waitCombo->ReadyForActivation();
		AGameplayAbilityTargetActor* targetActor;
		waitCombo->BeginSpawningActor(this, targetActorClass, targetActor);
		AActor* Avatar = GetAvatarActorFromActorInfo();
		targetActor->PostSpawnInitialize(Avatar->GetActorTransform(), Avatar, (APawn*)Avatar, false, true, true);
		waitCombo->FinishSpawningActor(this, targetActor);
	}
}

void UGA_UpperCut::ComboHit(FGameplayEventData Payload)
{
	TArray<AActor*> ComboTargets = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TargetDataHandle, 0);
	if (ComboTargets.Num() == 0) return;

	AActor* avatar = GetAvatarActorFromActorInfo();
	ACharacter* avatarAsCharacter = Cast<ACharacter>(avatar);
	avatarAsCharacter->LaunchCharacter(FVector::UpVector * ComboLaunchSpeed, true, true);

	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(ComboEffect, GetAbilityLevel()), TargetDataHandle);
	ApplyStunEffectToTarget(TargetDataHandle);
	for (AActor* target : ComboTargets)
	{
		if (target == avatar) continue;

		ACharacter* TargetAsCharacter = Cast<ACharacter>(target);
		if (TargetAsCharacter)
		{
			TargetAsCharacter->LaunchCharacter(FVector::UpVector * ComboLaunchSpeed, true, true);
		}
	}
}

void UGA_UpperCut::FinalBlow(FGameplayEventData Payload)
{
	TArray<AActor*> ComboTargets = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TargetDataHandle, 0);
	if (ComboTargets.Num() == 0) return;

	AActor* avatar = GetAvatarActorFromActorInfo();
	ACharacter* avatarAsCharacter = Cast<ACharacter>(avatar);
	avatarAsCharacter->GetCharacterMovement()->AddImpulse(-avatar->GetActorForwardVector() * FinalBlowPushSpeed, true);

	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(FinalBlowEffect, GetAbilityLevel()), TargetDataHandle);
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(FinalBlowStun, GetAbilityLevel()), TargetDataHandle);

	for (AActor* target : ComboTargets)
	{
		if (target == avatar) continue;

		ACharacter* TargetAsCharacter = Cast<ACharacter>(target);
		if (TargetAsCharacter)
		{
			TargetAsCharacter->GetCharacterMovement()->AddImpulse(avatar->GetActorForwardVector() * FinalBlowPushSpeed, true);
		}
	}
}

void UGA_UpperCut::LaunchTargets(const FGameplayAbilityTargetDataHandle& Data)
{
	TargetDataHandle = Data;
	TArray<AActor*> ComboTargets = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TargetDataHandle, 0);

	AActor* avatar = GetAvatarActorFromActorInfo();
	ACharacter* avatarAsCharacter = Cast<ACharacter>(avatar);
	avatarAsCharacter->LaunchCharacter(FVector::UpVector * LaunchSpeed, true, true);
	
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(LauchEffect, GetAbilityLevel()), TargetDataHandle);
	ApplyStunEffectToTarget(TargetDataHandle);

	for (AActor* target : ComboTargets)
	{
		if (target == avatar) continue;

		ACharacter* TargetAsCharacter = Cast<ACharacter>(target);
		if (TargetAsCharacter)
		{
			TargetAsCharacter->LaunchCharacter(FVector::UpVector * TargetLaunchSpeed, true, true);
		}
	}
}

void UGA_UpperCut::TryActivateCombo(const FGameplayAbilityTargetDataHandle& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("upper cut combo confirmed"))
	if (NextComboSectionName == NAME_None)
		return;

	USkeletalMeshComponent* mesh = GetOwningComponentFromActorInfo();
	if (mesh)
	{
		UAnimInstance* AnimBP = mesh->GetAnimInstance();
		if (AnimBP->Montage_GetCurrentSection() == NextComboSectionName)
		{
			return;
		}

		if (AnimBP && AnimBP->GetCurrentActiveMontage())
		{
			AnimBP->Montage_SetNextSection(AnimBP->Montage_GetCurrentSection(), NextComboSectionName, AnimBP->GetCurrentActiveMontage());
		}
	}
}

