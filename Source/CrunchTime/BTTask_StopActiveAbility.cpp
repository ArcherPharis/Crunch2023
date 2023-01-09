// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopActiveAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

EBTNodeResult::Type UBTTask_StopActiveAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComp.GetAIOwner()->GetPawn());
	if (AbilitySystemComp)
	{
		AbilitySystemComp->CancelAllAbilities();
	}
	return EBTNodeResult::Succeeded;
}