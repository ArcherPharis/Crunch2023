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

	UFUNCTION(BlueprintPure)
	float GetUpgradePoints() const;

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

	UPROPERTY()
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Strength);
	UPROPERTY()
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Intelligence);
	UPROPERTY()
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Armor);
	UPROPERTY()
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, AttackDamage);

	UPROPERTY()
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, MaxWalkSpeed);

	UPROPERTY()
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Experience);

	UPROPERTY()
	FGameplayAttributeData NextLevelExperience;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, NextLevelExperience);

	UPROPERTY()
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, Level);

	UPROPERTY()
	FGameplayAttributeData upgradePoint;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, upgradePoint);

	UPROPERTY()
	FGameplayAttributeData credit;
	ATTRIBUTE_ACCESSORS(UCTAttributeSet, credit);

private:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data);
};
