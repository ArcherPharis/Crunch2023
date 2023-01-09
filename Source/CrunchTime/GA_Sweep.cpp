// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Sweep.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
void UGA_Sweep::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
		return;

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	UAbilityTask_PlayMontageAndWait* PlaySweepMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SweepMontage);
	if (PlaySweepMontageTask)
	{
		PlaySweepMontageTask->OnBlendOut.AddDynamic(this, &UGA_Sweep::SweepFinished);
		PlaySweepMontageTask->OnCancelled.AddDynamic(this, &UGA_Sweep::SweepFinished);
		PlaySweepMontageTask->OnInterrupted.AddDynamic(this, &UGA_Sweep::SweepFinished);
		PlaySweepMontageTask->OnCompleted.AddDynamic(this, &UGA_Sweep::SweepFinished);
		PlaySweepMontageTask->ReadyForActivation();
	}
	UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::CustomMulti, TargetActorClass);
	if (WaitTargetDataTask)
	{
		WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_Sweep::TargetAquired);
		WaitTargetDataTask->ReadyForActivation();
	
		class AGameplayAbilityTargetActor* TargetActor;
		WaitTargetDataTask->BeginSpawningActor(this, TargetActorClass, TargetActor);
		TargetActor->SetOwner(GetAvatarActorFromActorInfo());
		WaitTargetDataTask->FinishSpawningActor(this, TargetActor);
		TargetActor->AttachToComponent(GetOwningComponentFromActorInfo(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSlotName);
	}
}

void UGA_Sweep::SweepFinished()
{
	K2_EndAbility();
}

void UGA_Sweep::TargetAquired(const FGameplayAbilityTargetDataHandle& Data)
{
	AActor* Target = Data.Get(0)->GetActors()[0].Get();
	if (Target)
	{
		FVector TargetDir = (Target->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
		PushActorAsCharacter(Target, TargetDir, pushSpeed);
	}
	K2_ApplyGameplayEffectSpecToTarget(MakeOutgoingGameplayEffectSpec(DamageEffectClass), Data);
	ApplyStunEffectToTarget(Data);
}
