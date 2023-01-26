// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidgetBase.h"
#include "ItemInventoryItemSlotWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemActivated, int);

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

	void ChangeColor();

	void EmptySlot();
	bool IsForItem(int handle);

	FOnItemActivated onItemActivated;

private:
	bool bisEmpty = true;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StackText;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UTexture2D* EmptyTexture;

	

	UFUNCTION()
	void StackChanged(int newStackCount);

	virtual void LeftClicked() override;

	int ItemSpecHandle = INDEX_NONE;
	
};
