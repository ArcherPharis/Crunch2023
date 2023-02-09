// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemSpec.h"
#include "GameplayEffect.h"
#include "Item.h"

FInventoryItemSpec::FInventoryItemSpec(): ItemCDO{nullptr}, PassiveEffectHandle{}, stackCount{0}, handle{INDEX_NONE}
{
}

FInventoryItemSpec::FInventoryItemSpec(const FInventoryItemSpec& rhs)
	: ItemCDO{rhs.ItemCDO},
	PassiveEffectHandle{rhs.PassiveEffectHandle},
	stackCount{rhs.stackCount},
	handle{ rhs.handle },
	onStackChanged{rhs.onStackChanged}
	
{

}

FInventoryItemSpec::FInventoryItemSpec(const UItem* item, FActiveGameplayEffectHandle handle) : ItemCDO{item}, PassiveEffectHandle{handle}, stackCount{1}, handle{CreateNewHandle()}
{

}

bool FInventoryItemSpec::IsStackable() const
{
	return ItemCDO->IsStackable();
}

bool FInventoryItemSpec::IsConsumable() const
{
	return ItemCDO->IsConsumable();
}

void FInventoryItemSpec::PushStack()
{
	++stackCount;
	onStackChanged.Broadcast(stackCount);
}

bool FInventoryItemSpec::PopStack()
{
	--stackCount;
	onStackChanged.Broadcast(stackCount);
	return stackCount > 0;
}

const UGameplayEffect* FInventoryItemSpec::GetItemPassiveEffect() const
{

	auto passiveEffect = ItemCDO->GetPassiveGameplayEffect();

	if (passiveEffect)
	{
		return passiveEffect.GetDefaultObject();
	}
	return nullptr;
}

const TSubclassOf<UGameplayAbility> FInventoryItemSpec::GetGrantedAbilityClass() const
{
	const UGameplayEffect* passiveEffectCDO = GetItemPassiveEffect();

	if (passiveEffectCDO && passiveEffectCDO->GrantedAbilities.Num() != 0)
	{
		return passiveEffectCDO->GrantedAbilities[0].Ability;
	}
	return TSubclassOf<UGameplayAbility>();
}

int FInventoryItemSpec::CreateNewHandle() const
{
	static int InventoryItemHandle = 0;
	return InventoryItemHandle++;
}

