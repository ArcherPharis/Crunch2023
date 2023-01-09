// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DoBasicAttack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "CTCharacterBase.h"
EBTNodeResult::Type UBTTask_DoBasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACTCharacterBase* PawnAsCharacterBase = Cast<ACTCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (PawnAsCharacterBase)
	{
		PawnAsCharacterBase->DoBasicAttack();
	}
	return EBTNodeResult::Succeeded;
}