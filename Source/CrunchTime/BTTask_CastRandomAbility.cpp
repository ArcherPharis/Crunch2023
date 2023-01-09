// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CastRandomAbility.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTCharacterBase.h"

UBTTask_CastRandomAbility::UBTTask_CastRandomAbility()
{
	NodeName = "Cast Random Abiilty";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_CastRandomAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACTCharacterBase* PawnAsCharacterBase = Cast<ACTCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	auto abilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PawnAsCharacterBase);
	if (PawnAsCharacterBase && abilitySystemComp)
	{
		int randomIndex = FMath::RandRange(0, possibleAbilitys.Num()-1);
		if (abilitySystemComp->TryActivateAbilityByClass(PawnAsCharacterBase->GetAbilityByIndex(possibleAbilitys[randomIndex])))
		{
			if (waitTimeAfterCasting == 0)
			{
				return EBTNodeResult::Succeeded;
			}
			else
			{
				timer = 0.f;
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_CastRandomAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	timer += DeltaSeconds;
	if (timer >= waitTimeAfterCasting)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
