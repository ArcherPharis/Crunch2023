// Fill out your copyright notice in the Description page of Project Settings.


#include "AICEnemyBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
AAICEnemyBase::AAICEnemyBase()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightConfig");
	if (SightConfig)
	{
		PerceptionComp->ConfigureSense(*SightConfig);
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAICEnemyBase::PerceptionUpdated);
	}
}

void AAICEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	if (AlwaysKnowPlayer)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		GetBlackboardComponent()->SetValueAsObject(TargetBlackboardKeyName, PlayerPawn);
	}
}

void AAICEnemyBase::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (AlwaysKnowPlayer)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetBlackboardKeyName, Actor);
	}
	else
	{
		auto PerceptionInfo = PerceptionComp->GetActorInfo(*Actor);
		if (!PerceptionInfo->HasAnyCurrentStimulus())
		{
			//UE_LOG(LogTemp, Warning, TEXT("I lost track of: %s"), *Actor->GetName());
			//GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
		}
	}
}

void AAICEnemyBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	IGenericTeamAgentInterface* pawnInterface = Cast<IGenericTeamAgentInterface>(InPawn);
	if (pawnInterface)
	{
		TeamID = pawnInterface->GetGenericTeamId();
	}
}
