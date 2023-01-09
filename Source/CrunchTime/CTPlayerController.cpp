// Fill out your copyright notice in the Description page of Project Settings.


#include "CTPlayerController.h"
#include "CTCrunch.h"
#include "CTAttributeSet.h"
#include "InGameUI.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
void ACTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	if (InGameUIClass)
	{
		inGameUI = CreateWidget<UInGameUI>(this, InGameUIClass);
		inGameUI->OnResumeBtnClicked.AddDynamic(this, &ACTPlayerController::TogglePause);
		inGameUI->AddToViewport();
	}

	Crunch = Cast<ACTCrunch>(InPawn);
	if (Crunch)
	{
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &ACTPlayerController::HealthUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetStaminaAttribute()).AddUObject(this, &ACTPlayerController::StaminaUpdated);
		Crunch->ApplayInitialEffect();
		Crunch->OnAbilityGiven.AddUObject(this, &ACTPlayerController::PawnAddAbility);
		Crunch->GetAbilitySystemComponent()->AbilityCommittedCallbacks.AddUObject(this, &ACTPlayerController::AbilityActivated);
		Crunch->OnCharacterDeath.AddDynamic(this, &ACTPlayerController::PawnDead);
		Crunch->OnCharacterDeathStarted.AddDynamic(this, &ACTPlayerController::PawnDeathStarted);
	}
}

void ACTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		FInputActionBinding& PauseInputBinding = InputComponent->BindAction("TogglePause", IE_Pressed, this, &ACTPlayerController::TogglePause);
		PauseInputBinding.bExecuteWhenPaused = true;
	}
}

void ACTPlayerController::HealthUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateHealth(AttributeData.NewValue, Crunch->GetAttributeSet()->GetMaxHealth());
}

void ACTPlayerController::StaminaUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateStamina(AttributeData.NewValue, Crunch->GetAttributeSet()->GetMaxStamina());
}

void ACTPlayerController::PawnAddAbility(const FGameplayAbilitySpec* SpecHandle)
{
	inGameUI->AbilityAdded(SpecHandle->Ability);
}

void ACTPlayerController::AbilityActivated(UGameplayAbility* ability)
{
	inGameUI->AbilityActivated(ability);
}

void ACTPlayerController::PawnDeathStarted()
{
	inGameUI->DeathStarted();
}

void ACTPlayerController::PawnDead()
{
	if (bIsPawnDead)
	{
		return;
	}

	bIsPawnDead = true;
	UGameplayStatics::SetGamePaused(this, true);
	inGameUI->SwitchToDeathUI();
	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
}

void ACTPlayerController::TogglePause()
{
	if (bIsPawnDead)
	{
		return;
	}

	if (UGameplayStatics::IsGamePaused(this))
	{
		UGameplayStatics::SetGamePaused(this, false);
		inGameUI->SwitchToGameplayUI();

		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UGameplayStatics::SetGamePaused(this, true);
		inGameUI->SwitchToPauseUI();
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}
}
