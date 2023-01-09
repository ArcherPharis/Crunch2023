// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_Notifiy.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UGC_Notifiy : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
};
