// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityBase.h"
#include "GA_Sweep.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UGA_Sweep : public UCTGameplayAbilityBase
{
	GENERATED_BODY()
private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sweep")
	UAnimMontage* SweepMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sweep")
	TSubclassOf<class AGameplayAbilityTargetActor> TargetActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sweep")
	float pushSpeed = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sweep")
	FName AttachSlotName;

	UPROPERTY(EditDefaultsOnly, Category = "Sweep")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UFUNCTION()
	void SweepFinished();
	
	UFUNCTION()
	void TargetAquired(const FGameplayAbilityTargetDataHandle& Data);
};
