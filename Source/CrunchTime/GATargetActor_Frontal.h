// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityTargetActorBase.h"
#include "GATargetActor_Frontal.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API AGATargetActor_Frontal : public ACTGameplayAbilityTargetActorBase
{
	GENERATED_BODY()
public:
	AGATargetActor_Frontal();

private:
	/** Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests. */
	virtual void ConfirmTargetingAndContinue() override;

	UPROPERTY()
	class USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, Category = "Targetting")
	class UBoxComponent* TargetRange;
};
