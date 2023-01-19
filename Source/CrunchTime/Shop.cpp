// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"
#include "CTAssetManager.h"
#include "Item.h"

UShop* UShop::GetShop()
{
	UCTAssetManager& assetManager = UCTAssetManager::Get();
	return assetManager.GetShop();
}

void UShop::ASyncLoadShopItems()
{
	UCTAssetManager& assetManager = UCTAssetManager::Get();
	TArray<FPrimaryAssetId> assetIDs;
	assetManager.GetPrimaryAssetIdList(UCTAssetManager::InventoryItemType, assetIDs, EAssetManagerFilter::UnloadedOnly);

	//unless you need to make a copy, always use & or  if the type is a pointer, then & is not needed.
	for (const FPrimaryAssetId& itemAssetId : assetIDs)
	{
		assetManager.LoadPrimaryAsset(itemAssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UShop::AssetLoaded, itemAssetId));
	}
}

void UShop::AssetLoaded(const FPrimaryAssetId assetID)
{
	UCTAssetManager& assetManager = UCTAssetManager::Get();
	TSubclassOf<UItem> itemClass =  assetManager.GetPrimaryAssetObjectClass<UItem>(assetID);
	onItemLoaded.Broadcast(itemClass);
}
