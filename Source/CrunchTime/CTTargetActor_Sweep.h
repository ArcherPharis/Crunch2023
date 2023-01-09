// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityTargetActorBase.h"
#include "CTTargetActor_Sweep.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API ACTTargetActor_Sweep : public ACTGameplayAbilityTargetActorBase
{
	GENERATED_BODY()
public:
	ACTTargetActor_Sweep();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Sweep")
	class USceneComponent* RootComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sweep")
	class UCapsuleComponent* SweepTargetArea;

	UFUNCTION()
	void TargetAreaOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
