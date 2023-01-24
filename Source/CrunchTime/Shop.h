// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Shop.generated.h"

class UItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemLoaded, TSubclassOf<UItem>, itemLoaded);

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UShop : public UObject
{
	GENERATED_BODY()

public:

	FOnItemLoaded onItemLoaded;

	static UShop* GetShop();

	void ASyncLoadShopItems();
	void AssetLoaded(const FPrimaryAssetId assetID);
	void ProcessPurchaseEvent(const UItem* itemToPurchase, AActor* Purchaser);
	
};
