// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "Shop.h"
#include "Components/TileView.h"
#include "Item.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UShop::GetShop()->onItemLoaded.AddDynamic(this, &UShopWidget::ItemLoaded);
	UShop::GetShop()->ASyncLoadShopItems();
}

void UShopWidget::ItemLoaded(TSubclassOf<class UItem> itemClass)
{
	tileView->AddItem(itemClass);
}
