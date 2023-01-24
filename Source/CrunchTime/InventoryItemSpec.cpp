// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemSpec.h"

FInventoryItemSpec::FInventoryItemSpec(): ItemCDO{nullptr}, PassiveEffectHandle{}, stackCount{0}
{
}

FInventoryItemSpec::FInventoryItemSpec(const FInventoryItemSpec& rhs)
	: ItemCDO{rhs.ItemCDO},
	PassiveEffectHandle{rhs.PassiveEffectHandle},
	stackCount{rhs.stackCount}
{

}

FInventoryItemSpec::FInventoryItemSpec(const UItem* item, FActiveGameplayEffectHandle handle) : ItemCDO{item}, PassiveEffectHandle{handle}, stackCount{1}
{

}

