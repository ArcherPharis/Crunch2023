// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAttributeSet.h"
#include "GameplayEffectExtension.h"

void UCTAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxStamina());
	}

	if (Attribute == GetcreditAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, TNumericLimits<float>::Max());
	}
}

void UCTAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0, GetMaxStamina()));
	}

	if (Data.EvaluatedData.Attribute == GetcreditAttribute())
	{
		Setcredit(FMath::Clamp(Getcredit(), 0, TNumericLimits<float>::Max()));
	}
}

float UCTAttributeSet::GetUpgradePoints() const
{
	return GetupgradePoint();
}

