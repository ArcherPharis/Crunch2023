// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Components/WrapBox.h"
#include "ItemInventoryItemSlotWidget.h"
#include "ItemInventoryItemSlotWidget.h"


void UInventoryWidget::ActivateItem(uint8 itemIndex)
{
	if (itemIndex >= Slots.Num()) return;
	Slots[itemIndex]->UseItem();
}

void UInventoryWidget::NativeConstruct()
{
	Super:: NativeConstruct();
	OwningInventoryComp = Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));
	OwningInventoryComp->onItemChanged.AddUObject(this, &UInventoryWidget::ItemChanged);
	BuildGrid();
}

void UInventoryWidget::ItemChanged(FInventoryItemSpec* spec, bool bWasAdded)
{
	if (bWasAdded)
	{
		for (auto slot : Slots)
		{
			if (slot->IsEmpty())
			{
				slot->AssignItem(spec);
				slot->UpdateItemCooldown();
				break;
			}
		}
		
	}
	else
	{
		for (auto slot : Slots)
		{
			if (slot->IsForItem(spec->GetHandle()))
			{
				slot->EmptySlot();
			}
		}
	}
	
}

void UInventoryWidget::BuildGrid()
{
	if (SlotClass == nullptr) return;
	SlotWrapBox->ClearChildren();
	int capacity = OwningInventoryComp->GetCapacity();

	for (int i = 0; i < capacity; ++i)
	{
		UItemInventoryItemSlotWidget* newSlot = CreateWidget<UItemInventoryItemSlotWidget>(SlotWrapBox, SlotClass);
		newSlot->onSwapRequested.AddUObject(this, &UInventoryWidget::SwapItems);
		newSlot->onItemActivated.AddUObject(OwningInventoryComp, &UInventoryComponent::ItemActivated);
		newSlot->onItemSold.AddUObject(OwningInventoryComp, &UInventoryComponent::ItemSold);
		newSlot->bIsFocusable = true;
		Slots.Add(newSlot);
		SlotWrapBox->AddChildToWrapBox(newSlot);

	}
}

void UInventoryWidget::SwapItems(UItemInventoryItemSlotWidget* itemA, UItemInventoryItemSlotWidget* itemB)
{
	FInventoryItemSpec* specA = OwningInventoryComp->GetItemSpec(itemA->GetHandle());
	FInventoryItemSpec* specB = OwningInventoryComp->GetItemSpec(itemB->GetHandle());
	itemA->AssignItem(specB);
	itemB->AssignItem(specA);

}
