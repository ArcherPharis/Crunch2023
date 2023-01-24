// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE UTexture2D* GetIconTexture() const { return iconTexture; }
	FORCEINLINE FText GetDescription() const { return description; }
	FORCEINLINE FText GetItemName() const { return name; }
	FORCEINLINE TSubclassOf<class UItemToolTip> GetToolTipClass() const { return toolTipClass; }
	FORCEINLINE float GetPrice() const { return price; }
	FORCEINLINE TSubclassOf<UGameplayEffect> GetPassiveGameplayEffect() const { return EquipPassiveEffect; }


private:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FPrimaryAssetType assetType;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	float price;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UTexture2D* iconTexture;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText name;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText description;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UGameplayEffect> EquipPassiveEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UGameplayEffect> ActivationEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool isStackable;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool isConsumable;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UItemToolTip> toolTipClass;

	
};
