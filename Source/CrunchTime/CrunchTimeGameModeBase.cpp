// Copyright Epic Games, Inc. All Rights Reserved.


#include "CrunchTimeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ACrunchTimeGameModeBase::BackToMainMenu() const
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
}

void ACrunchTimeGameModeBase::QuitGame() const
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, true);
}

void ACrunchTimeGameModeBase::RestartCurrentLevel() const
{
	FString currentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(currentLevelName));
}
