// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryItemSlotWidget.h"
#include "InventoryItemSpec.h"

void UItemInventoryItemSlotWidget::AssignItem(FInventoryItemSpec* spec)
{
	InitFromItem(spec->GetItem());
	bisEmpty = false;
}

bool UItemInventoryItemSlotWidget::IsEmpty() const
{
	return bisEmpty;
}
