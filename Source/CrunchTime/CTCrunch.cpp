// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCrunch.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"

ACTCrunch::ACTCrunch()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	PlayerEye = CreateDefaultSubobject<UCameraComponent>("PlayerEye");
	PlayerEye->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void ACTCrunch::BeginPlay()
{
	Super::BeginPlay();
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength, ZoomRange.X, ZoomRange.Y);
}

void ACTCrunch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACTCrunch::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACTCrunch::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACTCrunch::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ACTCrunch::Turn);
	PlayerInputComponent->BindAxis("Zoom", this, &ACTCrunch::Zoom);
	PlayerInputComponent->BindAction("Jump", IE_Pressed , this, &ACTCrunch::Jump);
	PlayerInputComponent->BindAction("BasicAttack", IE_Pressed , this, &ACTCrunch::BasicAttack);
	GetAbilitySystemComponent()->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), FString("ECTAbilityInputID"), (int32)ECTAbilityInputID::Confirm,(int32)ECTAbilityInputID::Cancel));
}

void ACTCrunch::MoveForward(float amt)
{
	AddMovementInput(FVector(PlayerEye->GetForwardVector().X, PlayerEye->GetForwardVector().Y, 0).GetSafeNormal(), amt);
}

void ACTCrunch::MoveRight(float amt)
{
	AddMovementInput(PlayerEye->GetRightVector(), amt);
}

void ACTCrunch::LookUp(float amt)
{
	AddControllerPitchInput(amt);
}

void ACTCrunch::Turn(float amt)
{
	AddControllerYawInput(amt);
}

void ACTCrunch::Zoom(float amt)
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - amt * zoomAmt, ZoomRange.X, ZoomRange.Y);
}
