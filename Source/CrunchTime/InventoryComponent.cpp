// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "CTAbilitySystemBlueprintLibrary.h"
#include "Item.h"
#include "AbilitySystemComponent.h"
#include "CTAttributeSet.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInventoryComponent::ItemActivated(int itemHandle)
{
	FInventoryItemSpec* spec = ItemContainer.Find(itemHandle);
	if (spec)
	{
		ApplyItemActiveEffect(spec->GetItem());
		if (spec->IsConsumable() && !spec->PopStack())
		{
			onItemChanged.Broadcast(spec, false);
			ItemContainer.Remove(itemHandle);
		}
	}
}

void UInventoryComponent::ItemSold(int itemHandle)
{
	FInventoryItemSpec* spec = ItemContainer.Find(itemHandle);
	if (spec)
	{
		float price = spec->GetItem()->GetPrice();
		bool found;
		float avaliableCredit = OwnerAbilitySystemComp->GetGameplayAttributeValue(UCTAttributeSet::GetcreditAttribute(), found);
		OwnerAbilitySystemComp->SetNumericAttributeBase(UCTAttributeSet::GetcreditAttribute(), avaliableCredit + price/2);
		if (spec->IsConsumable() && !spec->PopStack())
		{
			onItemChanged.Broadcast(spec, false);
			ItemContainer.Remove(itemHandle);
		}
	}

}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FInventoryItemSpec* UInventoryComponent::GetItemSpec(int handle)
{
	return ItemContainer.Find(handle);
}


void UInventoryComponent::GiveItem(const UItem* itemCDO)
{

	//adding onto a stack of item that can stack, make sure its a reference
	for (auto& pair : ItemContainer)
	{
		if (pair.Value.GetItem() == itemCDO && pair.Value.IsStackable())
		{
			pair.Value.PushStack();
			return;
		}
	}

	//adding a new one
	FActiveGameplayEffectHandle handle = ApplyItemPassiveEffect(itemCDO);
	FInventoryItemSpec newItemSpec = FInventoryItemSpec(itemCDO, handle);
	onItemChanged.Broadcast(&ItemContainer.Add(newItemSpec.GetHandle(), newItemSpec), true);
	//& made it into a address, hence a pointer!
}

void UInventoryComponent::SpendCredit(float amt)
{
	bool bFoundAttr;
	float credits = OwnerAbilitySystemComp->GetGameplayAttributeValue(UCTAttributeSet::GetcreditAttribute(), bFoundAttr);
	OwnerAbilitySystemComp->SetNumericAttributeBase(UCTAttributeSet::GetcreditAttribute(), credits - amt);
}

FActiveGameplayEffectHandle UInventoryComponent::ApplyItemPassiveEffect(const UItem* itemCDO) const
{
	TSubclassOf<UGameplayEffect> passiveEffect =  itemCDO->GetPassiveGameplayEffect();
	return ApplyGameplayEffectToOwner(passiveEffect);
}

FActiveGameplayEffectHandle UInventoryComponent::ApplyItemActiveEffect(const UItem* itemCDO) const
{
	TSubclassOf<UGameplayEffect> activeEffect = itemCDO->GetActiveGameplayEffect();
	return ApplyGameplayEffectToOwner(activeEffect);
}

bool UInventoryComponent::HasCreditFor(const UItem* item) const
{
	float price = item->GetPrice();
	bool bFoundAttr;
	float credits = OwnerAbilitySystemComp->GetGameplayAttributeValue(UCTAttributeSet::GetcreditAttribute(), bFoundAttr);
	
	return credits >= price;
}

void UInventoryComponent::TryPurchase(const UItem* ItemToPurchase)
{
	if (!HasCreditFor(ItemToPurchase)) return;
	if (isFull()) return;

	GiveItem(ItemToPurchase);
	SpendCredit(ItemToPurchase->GetPrice());
}

bool UInventoryComponent::isFull() const
{
	return false;
}

FActiveGameplayEffectHandle UInventoryComponent::ApplyGameplayEffectToOwner(TSubclassOf<UGameplayEffect> effectToApply) const
{
	if (effectToApply)
	{
		FGameplayEffectSpecHandle spec = OwnerAbilitySystemComp->MakeOutgoingSpec(effectToApply, 1, OwnerAbilitySystemComp->MakeEffectContext());
		return OwnerAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*spec.Data);
	}
	return FActiveGameplayEffectHandle();
}

