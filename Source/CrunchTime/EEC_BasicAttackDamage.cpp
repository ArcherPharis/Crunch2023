// Fill out your copyright notice in the Description page of Project Settings.


#include "EEC_BasicAttackDamage.h"
#include "CTAttributeSet.h"

struct BasicDamageCaptures
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	BasicDamageCaptures()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCTAttributeSet, AttackDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCTAttributeSet, Armor, Target, false);
	}
};

static const BasicDamageCaptures& GetDamageCapture()
{
	static BasicDamageCaptures DamageCaptureVar;
	return DamageCaptureVar;
}

UEEC_BasicAttackDamage::UEEC_BasicAttackDamage()
{
	RelevantAttributesToCapture.Add(GetDamageCapture().AttackDamageDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().ArmorDef);
}

void UEEC_BasicAttackDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	float AttackDamage = 0;
	FAggregatorEvaluateParameters params;
	params.SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	params.TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().AttackDamageDef, params, AttackDamage);

	float armor = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ArmorDef, params, armor);
	armor = FMath::Max(armor, 0);

	float finalDamage = AttackDamage - armor;
	finalDamage = FMath::Max(finalDamage, 0);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCTAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -finalDamage));


}
