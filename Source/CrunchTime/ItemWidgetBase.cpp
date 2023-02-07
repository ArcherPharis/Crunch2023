// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidgetBase.h"
#include "Components/Image.h"
#include "ItemToolTip.h"
#include "Item.h"


void UItemWidgetBase::InitFromItemClass(TSubclassOf<UItem> itemClass)
{
	UItem* itemCDO = itemClass.GetDefaultObject();
	UItemToolTip* tt = CreateWidget<UItemToolTip>(this, itemCDO->GetToolTipClass());
	ItemIcon->GetDynamicMaterial()->SetTextureParameterValue(IconTexturePropertyName, itemCDO->GetIconTexture());
	tt->SetParameters(itemCDO->GetItemName(), itemCDO->GetDescription(), itemCDO->GetIconTexture());
	ItemIcon->SetToolTip(tt);
	
}

void UItemWidgetBase::InitFromItem(const UItem* item)
{
	UTexture* iconTexture = item->GetIconTexture();
	UE_LOG(LogTemp, Warning, TEXT("Item has texture: %s, will be assigend to: %s"), *iconTexture->GetName(), *IconTexturePropertyName.ToString())
	UItemToolTip* tt = CreateWidget<UItemToolTip>(this, item->GetToolTipClass());
	SetIconTexture(item->GetIconTexture());
	tt->SetParameters(item->GetItemName(), item->GetDescription(), item->GetIconTexture());
	ItemIcon->SetToolTip(tt);
	UE_LOG(LogTemp, Warning, TEXT("Reaching here: %s"), *item->GetName());
}

void UItemWidgetBase::InitFromOther(UItemWidgetBase* otherWidget)
{
	UTexture* otherTexture;
	otherWidget->GetIcon()->GetDynamicMaterial()->GetTextureParameterValue(IconTexturePropertyName, otherTexture);
	SetIconTexture(otherTexture);
	UE_LOG(LogTemp, Warning, TEXT("getting texture %s"), *otherTexture->GetName());
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

void UItemWidgetBase::SetIconTexture(UTexture* texture)
{

	GetIcon()->GetDynamicMaterial()->SetTextureParameterValue(IconTexturePropertyName, texture);

}

void UItemWidgetBase::RightClicked()
{
	
}

void UItemWidgetBase::LeftClicked()
{

}
