// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ActivateItem(uint8 itemIndex);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* SlotWrapBox;

	UPROPERTY()
	class UInventoryComponent* OwningInventoryComp;

	void ItemChanged(struct FInventoryItemSpec* spec, bool bWasAdded);

	void BuildGrid();

	

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<class UItemInventoryItemSlotWidget> SlotClass;


	void SwapItems(UItemInventoryItemSlotWidget* itemA, UItemInventoryItemSlotWidget* itemB);

	UPROPERTY()
	TArray<UItemInventoryItemSlotWidget*> Slots;
};
