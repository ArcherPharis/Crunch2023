// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityBase.h"
#include "GA_GroundBlastBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UGA_GroundBlastBase : public UCTGameplayAbilityBase
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	UAnimMontage* CastingMontage;

	UFUNCTION()
	void CastingAnimFinished();

	/**************************************************************************************/
	/*                                     Targeting                                      */
	/**************************************************************************************/
private:
	UFUNCTION()
	void StartTargeting(FGameplayEventData Payload);

	UFUNCTION()
	void TargetAquired(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void TargetCancelled(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void TargetingMontageOut();

	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	FGameplayTag StartTargetingTag;

	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	TSubclassOf<class AGATargetActor_GroundPick> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	UAnimMontage* TargetingMontage;

	class UAbilityTask_PlayMontageAndWait* CastTargetingMontageTask;

	/**************************************************************************************/
	/*                                     Blasting                                       */
	/**************************************************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	FGameplayTag BlastGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	float BlastLaunchSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "GroundBlast")
	TSubclassOf<UGameplayEffect> LauchBlastEffect;

	void LaunchActorAsCharacter(AActor* Actor, float speed);

};
