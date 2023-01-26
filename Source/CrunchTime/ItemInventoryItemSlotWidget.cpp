// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryItemSlotWidget.h"
#include "InventoryItemSpec.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemInventoryItemSlotWidget::AssignItem(FInventoryItemSpec* spec)
{
	InitFromItem(spec->GetItem());
	spec->onStackChanged.AddDynamic(this, &UItemInventoryItemSlotWidget::StackChanged);
	bisEmpty = false;
	ItemSpecHandle = spec->GetHandle();
	if (spec->IsStackable())
	{
		StackText->SetVisibility(ESlateVisibility::Visible);
		StackChanged(1);
	}
	else
	{
		StackText->SetVisibility(ESlateVisibility::Hidden);

	}
}

bool UItemInventoryItemSlotWidget::IsEmpty() const
{
	return ItemSpecHandle == INDEX_NONE;
}

void UItemInventoryItemSlotWidget::ChangeColor()
{

}

void UItemInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EmptySlot();
}

void UItemInventoryItemSlotWidget::EmptySlot()
{
	SetIconTexture(EmptyTexture);
	StackText->SetVisibility(ESlateVisibility::Hidden);
	ItemSpecHandle = INDEX_NONE;

}

bool UItemInventoryItemSlotWidget::IsForItem(int handle)
{
	return ItemSpecHandle == handle;
}

void UItemInventoryItemSlotWidget::StackChanged(int newStackCount)
{
	UE_LOG(LogTemp, Warning, TEXT("Reached here"));
	StackText->SetText(FText::AsNumber(newStackCount));
}

void UItemInventoryItemSlotWidget::LeftClicked()
{
	Super::LeftClicked();
	onItemActivated.Broadcast(ItemSpecHandle);
}


