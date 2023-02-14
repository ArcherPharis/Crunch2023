// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryItemSlotWidget.h"
#include "InventoryItemSpec.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySlotMenuWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InventoryItemSpec.h"
#include "Blueprint/DragDropOperation.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Item.h"
#include "CTAbilitySystemBlueprintLibrary.h"

void UItemInventoryItemSlotWidget::AssignItem(FInventoryItemSpec* spec)
{

	if (spec == nullptr)
	{
		EmptySlot();
		return;
	}
	
	InitFromItem(spec->GetItem());
	spec->onStackChanged.Clear();
	UpdateAbilityInfoFromSpec(spec);
	spec->onStackChanged.AddDynamic(this, &UItemInventoryItemSlotWidget::StackChanged);
	bisEmpty = false;
	ItemSpecHandle = spec->GetHandle();
	if (spec->IsStackable())
	{
		StackText->SetVisibility(ESlateVisibility::Visible);
		StackChanged(spec->GetStackCount());
	}
	else
	{
		StackText->SetVisibility(ESlateVisibility::Hidden);

	}

	UpdateItemCooldown();
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
	UAbilitySystemComponent* abilitySystemComponent =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());

	if (abilitySystemComponent)
	{
		abilitySystemComponent->AbilityCommittedCallbacks.AddUObject(this, &UItemInventoryItemSlotWidget::AbilityActivated);
	}
}

void UItemInventoryItemSlotWidget::AbilityActivated(UGameplayAbility* ability)
{
	if (GrantedAbilityClass && ability->GetClass() == GrantedAbilityClass)
	{
		UCTAbilitySystemBlueprintLibrary::GetAbilityCooldownDurationAndTimeRemaining(GrantedAbilityClass, GetOwningPlayerPawn(), coolDownDuration, coolDownTimeRemaining);
		StartAbilityCooldown();
	}
}

void UItemInventoryItemSlotWidget::EmptySlot()
{
	SetIconTexture(EmptyTexture);
	
	StackText->SetVisibility(ESlateVisibility::Hidden);
	ItemSpecHandle = INDEX_NONE;
	GrantedAbilityClass = TSubclassOf<UGameplayAbility>();

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
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	GetIcon()->GetDynamicMaterial()->SetScalarParameterValue("Percent", 1);
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

void UItemInventoryItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("Drag Detected"));
	UDragDropOperation* newOperation = NewObject<UDragDropOperation>(this);
	UItemWidgetBase* dragVisual = CreateWidget<UItemWidgetBase>(GetOwningPlayer(), DragVisualClass);
	newOperation->Payload = this;
	dragVisual->InitFromOther(this);
	newOperation->DefaultDragVisual = dragVisual;
	OutOperation = newOperation;
}

void UItemInventoryItemSlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	
}

void UItemInventoryItemSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("Drag Leaving"));
}

bool UItemInventoryItemSlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("Drag Over"));
	return true;
}

bool UItemInventoryItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("Drop Detected"));

	if (InOperation)
	{
		UItemInventoryItemSlotWidget* otherSlotWidget =  Cast<UItemInventoryItemSlotWidget>(InOperation->Payload);
		onSwapRequested.Broadcast(this, otherSlotWidget);
	}

	return true;
}

void UItemInventoryItemSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("Drag Cancelled"));
}

void UItemInventoryItemSlotWidget::UpdateAbilityInfoFromSpec(const FInventoryItemSpec* spec)
{
	GrantedAbilityClass = spec->GetGrantedAbilityClass();
	UCTAbilitySystemBlueprintLibrary::GetAbilityCooldownDurationAndTimeRemaining(GrantedAbilityClass, GetOwningPlayerPawn(), coolDownDuration, coolDownTimeRemaining);

	if (coolDownTimeRemaining != 0)
	{
		StartAbilityCooldown();
	}
}

void UItemInventoryItemSlotWidget::StartAbilityCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("ABILITY COOLDOWN STARTED"));
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UItemInventoryItemSlotWidget::UpdateItemCooldown, CooldownUpdateRate, true);
}

void UItemInventoryItemSlotWidget::UpdateItemCooldown()
{
	coolDownTimeRemaining -= CooldownUpdateRate;
	if (coolDownTimeRemaining > 0)
	{
		float coolDownPercentage = 1 - coolDownTimeRemaining / coolDownDuration;
		UE_LOG(LogTemp, Warning, TEXT("remaining: %f, duration: %f"), coolDownTimeRemaining, coolDownDuration);
		GetIcon()->GetDynamicMaterial()->SetScalarParameterValue(CooldownMaterialPropertyName, coolDownPercentage);

	}
	else
	{
		GetIcon()->GetDynamicMaterial()->SetScalarParameterValue(CooldownMaterialPropertyName, 1);
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}


