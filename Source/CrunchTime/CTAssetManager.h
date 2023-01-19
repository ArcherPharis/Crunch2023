// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CTAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UCTAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static FPrimaryAssetType InventoryItemType;
	static UCTAssetManager& Get();
	class UShop* GetShop();

	UPROPERTY()
	class UShop* shopSingleton;
	
};
