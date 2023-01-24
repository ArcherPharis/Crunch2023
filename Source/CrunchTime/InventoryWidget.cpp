// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "Item.h"
#include "Components/WrapBox.h"
#include "ItemInventoryItemSlotWidget.h"

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
				break;
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
		Slots.Add(newSlot);
		SlotWrapBox->AddChildToWrapBox(CreateWidget(SlotWrapBox, SlotClass));

	}
}
