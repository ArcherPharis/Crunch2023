// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CTAbilitySystemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UCTAbilitySystemBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
public:
	/*get the last part of the game play tag
		- example if tag is gameplay.commbo.combo01, then the return value will be: combo01
	*/
	static FName GetTagNameLast(const FGameplayTag& Tag);
	static FName GetRandomNameFromTagContainer(const FGameplayTagContainer& container);
};
