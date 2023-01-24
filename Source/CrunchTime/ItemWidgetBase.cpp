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

void UItemWidgetBase::InitFromItem(const UItem* item)
{
	UItemToolTip* tt = CreateWidget<UItemToolTip>(this, item->GetToolTipClass());
	Icon->GetDynamicMaterial()->SetTextureParameterValue(IconTexturePropertyName, item->GetIconTexture());
	tt->SetParameters(item->GetItemName(), item->GetDescription(), item->GetIconTexture());
	Icon->SetToolTip(tt);
}

FReply UItemWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//if not return, the detectDrags drag event will not fire
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		//if not return handled, OnMouseButtonUp will not be called.
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UItemWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (HasAnyUserFocus())
	{
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			LeftClicked();
		}
		else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			RightClicked();
		}
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UItemWidgetBase::RightClicked()
{
	
}

void UItemWidgetBase::LeftClicked()
{

}
