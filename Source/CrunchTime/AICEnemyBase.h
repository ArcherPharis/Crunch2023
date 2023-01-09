// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GenericTeamAgentInterface.h"
#include "AICEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API AAICEnemyBase : public AAIController
{
	GENERATED_BODY()
public:
	AAICEnemyBase();

	/**************************************************************************************/
	/*                                       TEAM                                         */
	/**************************************************************************************/
public:
	/** Assigns Team Agent to given TeamID */
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& ID) { TeamID = ID; }

	/** Retrieve team identifier in form of FGenericTeamId */
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const { return TeamID; }
private:
	FGenericTeamId TeamID;


	/**************************************************************************************/
	/*                                   Overrides                                        */
	/**************************************************************************************/
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;


	/**************************************************************************************/
	/*                                AIPerception                                        */
	/**************************************************************************************/
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* PerceptionComp;
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool AlwaysKnowPlayer = true;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetBlackboardKeyName {"Target"};

	/**************************************************************************************/
	/*                                BehaviorTree                                        */
	/**************************************************************************************/
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

};
