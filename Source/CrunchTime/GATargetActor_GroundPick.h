// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityTargetActorBase.h"
#include "GATargetActor_GroundPick.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API AGATargetActor_GroundPick : public ACTGameplayAbilityTargetActorBase
{
	GENERATED_BODY()
public:
	AGATargetActor_GroundPick();
protected:
	/** Initialize and begin targeting logic  */
	virtual void StartTargeting(UGameplayAbility* Ability);

	/** Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests. */
	virtual void ConfirmTargetingAndContinue();

	virtual void Tick(float deltaTime) override;

private:
	UPROPERTY()
	class USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, Category = "TargetActor")
	class USphereComponent* TargetArea;
};
