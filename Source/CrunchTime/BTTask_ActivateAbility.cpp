// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ActivateAbility.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTCharacterBase.h"
EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACTCharacterBase* PawnAsCharacterBase = Cast<ACTCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	auto abilitySystemComp =	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PawnAsCharacterBase);
	if (PawnAsCharacterBase && abilitySystemComp)
	{
		if (abilitySystemComp->TryActivateAbilityByClass(PawnAsCharacterBase->GetAbilityByIndex(AbilityIndex)))
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}