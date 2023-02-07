// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidgetBase.h"
#include "ItemInventoryItemSlotWidget.generated.h"

class UItemInventoryItemSlotWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemActivated, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemSelled, int);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSwapRequested, UItemInventoryItemSlotWidget*, UItemInventoryItemSlotWidget*);

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UItemInventoryItemSlotWidget : public UItemWidgetBase
{
	GENERATED_BODY()
public:
	void AssignItem(struct FInventoryItemSpec* spec);

	FOnSwapRequested onSwapRequested;

	bool IsEmpty() const;

	void ChangeColor();

	void EmptySlot();
	bool IsForItem(int handle);

	int GetHandle() const { return ItemSpecHandle; }


	FOnItemActivated onItemActivated;
	FOnItemSelled onItemSold;

private:
	bool bisEmpty = true;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StackText;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UTexture2D* EmptyTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf <UItemWidgetBase> DragVisualClass;

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

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
