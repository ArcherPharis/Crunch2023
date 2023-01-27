// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidgetBase.h"
#include "ItemInventoryItemSlotWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemActivated, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemSelled, int);

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
	FOnItemSelled onItemSold;

private:
	bool bisEmpty = true;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StackText;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UTexture2D* EmptyTexture;

	UPROPERTY(EditDefaultsOnly, Category = "InvnentoryItemSlot")
	TSubclassOf<class UInventorySlotMenuWidget> RightMenuWidgetClass;
	void SpawnRightClickMenu();
	class UInventorySlotMenuWidget* RightMenuWidget;

	void HideRightMenu();
	void ShowRightMenu();

	

	UFUNCTION()
	void StackChanged(int newStackCount);
	UFUNCTION()
	void SellItem();
	UFUNCTION()
	void UseItem();

	virtual void LeftClicked() override;
	virtual void RightClicked() override;

	int ItemSpecHandle = INDEX_NONE;
	
};
