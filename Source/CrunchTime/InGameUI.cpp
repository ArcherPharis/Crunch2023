// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "CrunchTimeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Headshot.h"

void UInGameUI::ChangeHeadshotImage()
{
	headShotImage->DesaturateImage();
}

void UInGameUI::HeadshotDamageReact()
{
	headShotImage->ReactToTakingDamage();
}

void UInGameUI::ResumeBtnClicked()
{
	OnResumeBtnClicked.Broadcast();
}

void UInGameUI::BackToMainMenuBtnClicked()
{
	if (GameMode)
	{
		GameMode->BackToMainMenu();
	}
}

void UInGameUI::QuitGameBtnClicked()
{
	if (GameMode)
	{
		GameMode->QuitGame();
	}
}

void UInGameUI::RestartBtnClicked()
{
	if (GameMode)
	{
		GameMode->RestartCurrentLevel();
	}
}

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	GameMode = Cast<ACrunchTimeGameModeBase>(UGameplayStatics::GetGameMode(this));
}
