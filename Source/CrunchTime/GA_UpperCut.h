// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityBase.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GA_UpperCut.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UGA_UpperCut : public UCTGameplayAbilityBase
{
	GENERATED_BODY()
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	FGameplayTag CutStartEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	FGameplayTag ComboHitEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	FGameplayTag ComboFinalBlowEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	FGameplayTag ComboEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	UAnimMontage* UpperCutMontage;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	TSubclassOf<class AGATargetActor_Frontal> targetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	float LaunchSpeed = 600;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	float ComboLaunchSpeed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	float TargetLaunchSpeed = 800;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	float FinalBlowPushSpeed = 300;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	class TSubclassOf<UGameplayEffect> LauchEffect;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	class TSubclassOf<UGameplayEffect> ComboEffect;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	class TSubclassOf<UGameplayEffect> FinalBlowEffect;

	UPROPERTY(EditDefaultsOnly, Category = "UpperCut")
	class TSubclassOf<UGameplayEffect> FinalBlowStun;

	UFUNCTION()
	void MontageFinished();

	UFUNCTION()
	void UpperCutStart(FGameplayEventData Payload);
	
	UFUNCTION()
	void UpdateComb(FGameplayEventData Payload);

	UFUNCTION()
	void ComboHit(FGameplayEventData Payload);

	UFUNCTION()
	void FinalBlow(FGameplayEventData Payload);

	UFUNCTION()
	void LaunchTargets(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void TryActivateCombo(const FGameplayAbilityTargetDataHandle& Data);

	FName NextComboSectionName;
	UPROPERTY()
	FGameplayAbilityTargetDataHandle TargetDataHandle;
};
