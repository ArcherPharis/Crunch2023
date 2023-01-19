// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidgetBase.generated.h"

class UItem;
/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UItemWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitFromItemClass(TSubclassOf<UItem> itemClass);

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	UPROPERTY(EditDefaultsOnly)
	FName IconTexturePropertyName;

	TSubclassOf<UItem> item;
	
};
