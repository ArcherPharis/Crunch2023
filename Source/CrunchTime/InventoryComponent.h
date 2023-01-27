// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemSpec.h"
#include "InventoryComponent.generated.h"

//FInventoryItemSpec* itemSpec, bool bIsAdded;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, FInventoryItemSpec*, bool);

class UItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRUNCHTIME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	void TryPurchase(const UItem* ItemToPurchase);

	FOnItemChanged onItemChanged;

	FORCEINLINE int GetCapacity() const { return capacity; }

	void ItemActivated(int itemHandle);
	void ItemSold(int itemHandle);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool HasCreditFor(const UItem* item) const;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void GiveItem(const UItem* itemCDO);
	void SpendCredit(float amt);

	FActiveGameplayEffectHandle ApplyItemPassiveEffect(const UItem* itemCDO) const;

	FActiveGameplayEffectHandle ApplyItemActiveEffect(const UItem* itemCDO) const;

	UPROPERTY()
	class UAbilitySystemComponent* OwnerAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int capacity = 6;

	bool isFull() const;

	//int is the handle
	UPROPERTY()
	TMap<int, FInventoryItemSpec> ItemContainer;

	FActiveGameplayEffectHandle ApplyGameplayEffectToOwner(TSubclassOf<class UGameplayEffect> effectToApply) const;

		
};
