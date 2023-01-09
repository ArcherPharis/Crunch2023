// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "MainMenu.generated.h"


/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetScreenResolution(const FIntPoint newRes, bool AutoApply = false);
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetFullScreenMode(TEnumAsByte<EWindowMode::Type> windowMode, bool AutoApply = false);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplyGameSettings();

	virtual void BeginConstruct();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool GetResolutionSetting(const FName& name, FIntPoint& outRes) const;
	
	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool GetWindowMode(const FName& name, TEnumAsByte<EWindowMode::Type>& outMode) const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	FName GetNameForRes(const FIntPoint& res) const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	FName GetNameForFullscreenMode(TEnumAsByte<EWindowMode::Type> type) const;

	template<typename T>
	FName GetKeyFromMap(const TMap<FName, T>& map, const T& val) const
	{
		for (auto pair : map)
		{
			if (val == pair.Value)
			{
				return pair.Key;
			}
		}

		return NAME_None;
	}

private:
	virtual void NativeConstruct() override;
	class UGameUserSettings* userSettings;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TMap<FName, FIntPoint> ResolutionSettings;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TMap<FName, TEnumAsByte<EWindowMode::Type>> windowModeSettings;
	
	UPROPERTY(meta = (BindWidget))
	class UComboBoxKey* ResolutionDropdown;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxKey* ScreenModeDropDown;

	void PullGameSettingToDropdownList();
};
