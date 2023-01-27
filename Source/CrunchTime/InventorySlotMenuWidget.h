// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "InventorySlotMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UInventorySlotMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnButtonClickedEvent& GetSellBtnClickDelegate() const;
	FOnButtonClickedEvent& GetUseBtnClickDelegate() const;

protected:
	virtual void NativeConstruct() override;


private:
	UPROPERTY(meta = (BindWidget))
		class UButton* SellBtn;

	UPROPERTY(meta = (BindWidget))
		class UButton* UseBtn;
	
};
