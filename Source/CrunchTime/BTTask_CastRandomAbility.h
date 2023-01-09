// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CastRandomAbility.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UBTTask_CastRandomAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CastRandomAbility();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	UPROPERTY(EditAnywhere, Category = "abilities")
	TArray<int> possibleAbilitys;

	UPROPERTY(EditAnywhere, Category = "abilities")
	float waitTimeAfterCasting = 0.f;

	float timer = 0.f;
};
