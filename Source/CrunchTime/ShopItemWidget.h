// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidgetBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ShopItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UShopItemWidget : public UItemWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void LeftClicked() override;
	virtual void RightClicked() override;

	TSubclassOf<UItem> itemClass;

	
};
