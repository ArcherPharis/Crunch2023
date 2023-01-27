// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryItemSlotWidget.h"
#include "InventoryItemSpec.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySlotMenuWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InventoryItemSpec.h"

void UItemInventoryItemSlotWidget::AssignItem(FInventoryItemSpec* spec)
{
	InitFromItem(spec->GetItem());
	spec->onStackChanged.AddDynamic(this, &UItemInventoryItemSlotWidget::StackChanged);
	bisEmpty = false;
	ItemSpecHandle = spec->GetHandle();
	if (spec->IsStackable())
	{
		StackText->SetVisibility(ESlateVisibility::Visible);
		StackChanged(1);
	}
	else
	{
		StackText->SetVisibility(ESlateVisibility::Hidden);

	}
}

bool UItemInventoryItemSlotWidget::IsEmpty() const
{
	return ItemSpecHandle == INDEX_NONE;
}

void UItemInventoryItemSlotWidget::ChangeColor()
{

}

void UItemInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EmptySlot();
	SpawnRightClickMenu();
}

void UItemInventoryItemSlotWidget::EmptySlot()
{
	SetIconTexture(EmptyTexture);
	StackText->SetVisibility(ESlateVisibility::Hidden);
	ItemSpecHandle = INDEX_NONE;

}

bool UItemInventoryItemSlotWidget::IsForItem(int handle)
{
	return ItemSpecHandle == handle;
}

void UItemInventoryItemSlotWidget::SpawnRightClickMenu()
{
	if (RightMenuWidget != nullptr) return;

	RightMenuWidget = CreateWidget<UInventorySlotMenuWidget>(this, RightMenuWidgetClass);

	RightMenuWidget->AddToViewport(3);
	RightMenuWidget->GetSellBtnClickDelegate().AddDynamic(this, &UItemInventoryItemSlotWidget::SellItem);
	RightMenuWidget->GetUseBtnClickDelegate().AddDynamic(this, &UItemInventoryItemSlotWidget::UseItem);
	HideRightMenu();
}

void UItemInventoryItemSlotWidget::HideRightMenu()
{
	RightMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UItemInventoryItemSlotWidget::ShowRightMenu()
{
	FVector2D PixelPos;
	FVector2D ViewportPosition;
	int viewportSizeX, viewportSizeY;
	GetOwningPlayer()->GetViewportSize(viewportSizeX, viewportSizeY);
	USlateBlueprintLibrary::AbsoluteToViewport(this, GetCachedGeometry().GetAbsolutePositionAtCoordinates(FVector2D(1.f, 0.5f)), PixelPos, ViewportPosition);
	float scale = UWidgetLayoutLibrary::GetViewportScale(RightMenuWidget);


	int overshoot = PixelPos.Y + RightMenuWidget->GetDesiredSize().Y * scale - viewportSizeY;
	if (overshoot > 0)
	{
		PixelPos.Y -= overshoot;
	}

	RightMenuWidget->SetPositionInViewport(PixelPos);

	RightMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void UItemInventoryItemSlotWidget::StackChanged(int newStackCount)
{
	UE_LOG(LogTemp, Warning, TEXT("Reached here"));
	StackText->SetText(FText::AsNumber(newStackCount));
}

void UItemInventoryItemSlotWidget::SellItem()
{
	onItemSold.Broadcast(ItemSpecHandle);
	HideRightMenu();
}

void UItemInventoryItemSlotWidget::UseItem()
{
	onItemActivated.Broadcast(ItemSpecHandle);
	HideRightMenu();
}

void UItemInventoryItemSlotWidget::LeftClicked()
{
	Super::LeftClicked();
	onItemActivated.Broadcast(ItemSpecHandle);
}

void UItemInventoryItemSlotWidget::RightClicked()
{
	if (!IsEmpty())
	{
		ShowRightMenu();
	}
}


