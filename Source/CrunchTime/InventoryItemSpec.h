// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "InventoryItemSpec.generated.h"

/**
 * 
 */
class UItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStackChanged, int, newStackCount);

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

	bool IsStackable() const;
	bool IsConsumable() const;
	void PushStack();
	bool PopStack();

	FORCEINLINE bool IsValid() const { return handle != INDEX_NONE; }
	FORCEINLINE int GetHandle() const { return handle; }

	FOnStackChanged onStackChanged;

private:
	const UItem* ItemCDO;
	TArray <FActiveGameplayEffectHandle> PassiveEffectHandle;
	int stackCount = 1;
	int handle;

	int CreateNewHandle() const;

};
