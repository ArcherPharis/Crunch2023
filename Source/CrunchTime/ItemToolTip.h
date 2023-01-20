// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTip.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UItemToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetParameters(FText skillName, FText skillDescription, UTexture2D* icon);

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* itemNameText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* itemDescriptionText;

	UPROPERTY(meta = (BindWidget))
	class UImage* itemIcon;
	
};
