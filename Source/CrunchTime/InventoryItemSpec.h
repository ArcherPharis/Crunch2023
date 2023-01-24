// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "InventoryItemSpec.generated.h"

/**
 * 
 */
class UItem;

USTRUCT(BlueprintType)
struct CRUNCHTIME_API FInventoryItemSpec
{
	GENERATED_BODY()

public:
	FInventoryItemSpec();

	//copy constructor
	FInventoryItemSpec(const FInventoryItemSpec& rhs);

	//custom constructor
	FInventoryItemSpec(const UItem* item, FActiveGameplayEffectHandle handle);

	FORCEINLINE const UItem* GetItem() const { return ItemCDO; }

private:
	const UItem* ItemCDO;
	TArray <FActiveGameplayEffectHandle> PassiveEffectHandle;
	int stackCount = 1;

};
