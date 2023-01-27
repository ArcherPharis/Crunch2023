// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotMenuWidget.h"

FOnButtonClickedEvent& UInventorySlotMenuWidget::GetSellBtnClickDelegate() const
{
	return SellBtn->OnClicked;
}

FOnButtonClickedEvent& UInventorySlotMenuWidget::GetUseBtnClickDelegate() const
{
	return UseBtn->OnClicked;
}

void UInventorySlotMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
