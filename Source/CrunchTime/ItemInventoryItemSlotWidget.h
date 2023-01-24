// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidgetBase.h"
#include "ItemInventoryItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UItemInventoryItemSlotWidget : public UItemWidgetBase
{
	GENERATED_BODY()
public:
	void AssignItem(struct FInventoryItemSpec* spec);

	bool IsEmpty() const;

private:
	bool bisEmpty = true;
	
};
