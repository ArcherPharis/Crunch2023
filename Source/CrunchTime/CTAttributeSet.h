// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CTAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UCTAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Health);

	UPROPERTY()
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, MaxHealth);

	UPROPERTY()
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Stamina);

	UPROPERTY()
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, MaxStamina);

private:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
};
