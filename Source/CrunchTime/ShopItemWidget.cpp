// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Shop.h"
#include "Item.h"

void UShopItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	itemClass = (Cast<UClass>(ListItemObject));
	InitFromItemClass(itemClass);
	bIsFocusable = true;
}

void UShopItemWidget::LeftClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Button Clicked"));
}

void UShopItemWidget::RightClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Button Clicked"));
	UShop::GetShop()->ProcessPurchaseEvent(itemClass.GetDefaultObject(), GetOwningPlayerPawn());
}

