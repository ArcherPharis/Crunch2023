// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidgetBase.h"
#include "Components/Image.h"
#include "ItemToolTip.h"
#include "Item.h"


void UItemWidgetBase::InitFromItemClass(TSubclassOf<UItem> itemClass)
{
	UItem* itemCDO = itemClass.GetDefaultObject();
	UItemToolTip* tt = CreateWidget<UItemToolTip>(this, itemCDO->GetToolTipClass());
	Icon->GetDynamicMaterial()->SetTextureParameterValue(IconTexturePropertyName, itemCDO->GetIconTexture());
	tt->SetParameters(itemCDO->GetItemName(), itemCDO->GetDescription(), itemCDO->GetIconTexture());
	Icon->SetToolTip(tt);
	
}
