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
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetStrengthAttribute()).AddUObject(this, &ACTPlayerController::StengthUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetAttackDamageAttribute()).AddUObject(this, &ACTPlayerController::AttackDamageUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetIntelligenceAttribute()).AddUObject(this, &ACTPlayerController::IntelligenceUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetArmorAttribute()).AddUObject(this, &ACTPlayerController::ArmorUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetMaxWalkSpeedAttribute()).AddUObject(this, &ACTPlayerController::WalkSpeedUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetLevelAttribute()).AddUObject(this, &ACTPlayerController::LevelUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetExperienceAttribute()).AddUObject(this, &ACTPlayerController::ExperienceUpdated);
		Crunch->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Crunch->GetAttributeSet()->GetNextLevelExperienceAttribute()).AddUObject(this, &ACTPlayerController::NextLevelExperienceUpdated);




		Crunch->OnAbilityGiven.AddUObject(this, &ACTPlayerController::PawnAddAbility);
		Crunch->GetAbilitySystemComponent()->AbilityCommittedCallbacks.AddUObject(this, &ACTPlayerController::AbilityActivated);
		Crunch->OnCharacterDeath.AddDynamic(this, &ACTPlayerController::PawnDead);
		Crunch->OnCharacterDeathStarted.AddDynamic(this, &ACTPlayerController::PawnDeathStarted);
		Crunch->OnCharacterTookDamage.AddDynamic(this, &ACTPlayerController::PawnTookDamage);
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

void ACTPlayerController::StengthUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateStrength(AttributeData.NewValue);
}

void ACTPlayerController::AttackDamageUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateAttackPower(AttributeData.NewValue);

}

void ACTPlayerController::IntelligenceUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateIntelligence(AttributeData.NewValue);

}

void ACTPlayerController::ArmorUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateArmor(AttributeData.NewValue);

}

void ACTPlayerController::WalkSpeedUpdated(const FOnAttributeChangeData& AttributeData)
{

	inGameUI->UpdateWalkSpeed(AttributeData.NewValue);

}

void ACTPlayerController::ExperienceUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateExperience(AttributeData.NewValue);
}
void ACTPlayerController::NextLevelExperienceUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateNextLevelExperience(AttributeData.NewValue);
}

void ACTPlayerController::LevelUpdated(const FOnAttributeChangeData& AttributeData)
{
	inGameUI->UpdateLevel(AttributeData.NewValue);
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
	inGameUI->ChangeHeadshotImage();
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

void ACTPlayerController::PawnTookDamage()
{
	inGameUI->HeadshotDamageReact();
}

void ACTPlayerController::UpdateAbilityIcon(UGameplayAbility* Ability)
{
	inGameUI->UpdateAbility(Ability);
}

void ACTPlayerController::LevelUpUI()
{
	inGameUI->LevelUpUI();
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
