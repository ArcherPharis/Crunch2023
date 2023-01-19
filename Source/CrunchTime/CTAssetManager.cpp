// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAssetManager.h"
#include "Shop.h"


FPrimaryAssetType UCTAssetManager::InventoryItemType(TEXT("InventoryItemType"));


UCTAssetManager& UCTAssetManager::Get()
{
	if (GEngine)
	{
		UCTAssetManager* managerSingleTon = Cast<UCTAssetManager>(GEngine->AssetManager);

		if (managerSingleTon)
		{
			return *managerSingleTon;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Wrong asset manager typed, need to be set to UCTAssetManager in the project settings"));
	return *NewObject<UCTAssetManager>();
}

UShop* UCTAssetManager::GetShop()
{
	if (shopSingleton == nullptr)
	{
		shopSingleton = NewObject<UShop>();
	}
	return shopSingleton;
}
