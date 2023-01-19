// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTCharacterBase.h"
#include "CTCrunch.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API ACTCrunch : public ACTCharacterBase
{
	GENERATED_BODY()
public:
	ACTCrunch();
	virtual void BeginPlay() override;

	/**************************************************************************************/
	/*                                    Components                                      */
	/**************************************************************************************/
private:
	UPROPERTY(VisibleAnywhere, Category = "Crunch")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* PlayerEye;


	/**************************************************************************************/
	/*                                    Input                                           */
	/**************************************************************************************/
private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* - Basic Control - */
	void MoveForward(float amt);
	void MoveRight(float amt);
	void LookUp(float amt);
	void Turn(float amt);
	
	/* - Zoom - */
	void Zoom(float amt);
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D ZoomRange {300.f, 1000.f};

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float zoomAmt = 10.f;

protected:
	UFUNCTION(BlueprintCallable, Category = "PlayerDebug")
	void TestLoadShopAssets();

};
