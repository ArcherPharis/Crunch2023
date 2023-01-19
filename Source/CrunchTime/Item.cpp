// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(assetType, GetFName());
}
