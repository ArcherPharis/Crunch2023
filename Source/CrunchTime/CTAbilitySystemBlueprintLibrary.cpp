// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FName UCTAbilitySystemBlueprintLibrary::GetTagNameLast(const FGameplayTag& Tag)
{
	FGameplayTag parentTag = Tag.RequestDirectParent();

	FString TagStr = Tag.ToString();
	FString ParentStr = parentTag.ToString() + ".";

	TagStr.RemoveAt(0, ParentStr.Len());
	return FName(TagStr);
}

FName UCTAbilitySystemBlueprintLibrary::GetRandomNameFromTagContainer(const FGameplayTagContainer& container)
{
	int tagCount = container.Num();
	if (tagCount == 0)
	{
		return NAME_None;
	}

	int comboPickIndex = FMath::RandRange(0, tagCount - 1);
	FGameplayTag pickedTag = container.GetByIndex(comboPickIndex);
	return GetTagNameLast(pickedTag);
}

void UCTAbilitySystemBlueprintLibrary::GetAbilityCooldownDurationAndTimeRemaining(TSubclassOf<UGameplayAbility> abilityClass, AActor* Caster, float& outDuration, float& outCooldownRemaining)
{
	outDuration = outCooldownRemaining = 0;
	UAbilitySystemComponent* ASC =  GetAbilitySystemComponent(Caster);

	if (!ASC) return;

	FGameplayAbilitySpec* abilitySpec = ASC->FindAbilitySpecFromClass(abilityClass);

	if (!abilitySpec) return;

	abilitySpec->Ability->GetCooldownTimeRemainingAndDuration(abilitySpec->Handle, ASC->AbilityActorInfo.Get(), outCooldownRemaining, outDuration);

}
