// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTGameplayAbilityTargetActorBase.h"
#include "GA_TargetActor_Beam.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API AGA_TargetActor_Beam : public ACTGameplayAbilityTargetActorBase
{
	GENERATED_BODY()
public:
	AGA_TargetActor_Beam();
	UFUNCTION(BlueprintCallable, Category = "Beam")
	FVector GetEndLoc() const;

	UFUNCTION(BlueprintCallable, Category = "Beem")
	float GetBeamLength() const;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	class USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	class USpringArmComponent* BeamBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	class UBoxComponent* TargetArea;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	float targetReportFrequency = 2.f;
		
	virtual void BeginPlay() override;

	UFUNCTION()
	void TargetAreaOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void TargetAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	TArray<AActor*> CurrentTargets;

	FTimerHandle timerHandle;

	void ReportTargets();
};
