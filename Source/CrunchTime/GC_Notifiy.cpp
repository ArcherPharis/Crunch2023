// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_Notifiy.h"

void UGC_Notifiy::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
	FGameplayCueParameters params = Parameters;
	params.RawMagnitude = 100;
	//UE_LOG(LogTemp, Warning,TEXT("I am now doing cue with target:%s, with maganitude of: %f"), *MyTarget->GetName(), Parameters.RawMagnitude);
}
