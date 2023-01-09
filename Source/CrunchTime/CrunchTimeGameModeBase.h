// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrunchTimeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API ACrunchTimeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void BackToMainMenu() const;
	void QuitGame() const;
	void RestartCurrentLevel() const;
private:
	UPROPERTY(EditDefaultsOnly, Category = "GameModeBase")
	TSoftObjectPtr<UWorld> MainMenuLevel;
};
