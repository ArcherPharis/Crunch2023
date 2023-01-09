// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CTGameplayAbilityTargetActorBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API ACTGameplayAbilityTargetActorBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
protected:
	bool IsActorHostileToAbilityOwner(AActor* otherActor) const;
};
