// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Item.h"

void UShopItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	TSubclassOf<UItem> itemClass(Cast<UClass>(ListItemObject));
	InitFromItemClass(itemClass);
}

