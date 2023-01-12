// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_LevelMaxHealth.h"
#include "CTAttributeSet.h"

UMMC_LevelMaxHealth::UMMC_LevelMaxHealth()
{
	StrengthCaptureDef.AttributeToCapture = UCTAttributeSet::GetStrengthAttribute();
	StrengthCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthCaptureDef);


}

float UMMC_LevelMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Strength = 0;
	GetCapturedAttributeMagnitude(StrengthCaptureDef, Spec, FAggregatorEvaluateParameters(), Strength);

	return 100.f + Strength * 5;
}
