// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTip.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemToolTip::SetParameters(FText skillName, FText skillDescription, UTexture2D* icon)
{
	itemNameText->SetText(skillName);
	itemDescriptionText->SetText(skillDescription);
	itemIcon->SetBrushFromTexture(icon);
}
