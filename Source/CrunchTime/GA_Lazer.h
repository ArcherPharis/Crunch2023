// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityBase.h"
#include "GA_Lazer.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UGA_Lazer : public UCTGameplayAbilityBase
{
	GENERATED_BODY()
private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Lazer")
	FName AttachSlotName;
	UPROPERTY(EditDefaultsOnly, Category = "Lazer")
	FGameplayTag StartLazerTag;
	UPROPERTY(EditDefaultsOnly, Category = "Lazer")
	UAnimMontage* CastingMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Lazer")
	float lazerPushSpeed = 4000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Lazer")
	TSubclassOf<class AGameplayAbilityTargetActor> LazerTargetingActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Lazer")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UFUNCTION()
	void CastingEnded();

	UFUNCTION()
	void TargetAquired(const FGameplayAbilityTargetDataHandle& Data);
	
	UFUNCTION()
	void TargetingCancelled();
	
	UFUNCTION()
	void StartCastingLazer(FGameplayEventData Payload);

	FActiveGameplayEffectHandle CostEffectHandle;
};
