// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidgetBase.h"
#include "Components/Image.h"
#include "Item.h"


void UItemWidgetBase::InitFromItemClass(TSubclassOf<UItem> itemClass)
{
	UItem* itemCDO = itemClass.GetDefaultObject();
	Icon->GetDynamicMaterial()->SetTextureParameterValue(IconTexturePropertyName, itemCDO->GetIconTexture());
}
