// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/ComboBoxKey.h"
#include "Components/WidgetSwitcher.h"
void UMainMenu::SetScreenResolution(const FIntPoint newRes, bool AutoApply)
{
	if (AutoApply)
	{
		ApplyGameSettings();
	}
}

void UMainMenu::SetFullScreenMode(TEnumAsByte<EWindowMode::Type> windowMode, bool AutoApply)
{
	if (windowMode == EWindowMode::WindowedFullscreen)
	{
		ResolutionDropdown->SetIsEnabled(false);
	}
	else
	{
		ResolutionDropdown->SetIsEnabled(true);
	}

	if (AutoApply)
	{
		ApplyGameSettings();
	}
}

void UMainMenu::ApplyGameSettings()
{
	if (!userSettings) return;

	FName screenResName = ResolutionDropdown->GetSelectedOption();
	FIntPoint res = userSettings->GetScreenResolution();
	if (GetResolutionSetting(screenResName, res))
	{
		userSettings->SetScreenResolution(res);
	}

	FName windowModeName = ScreenModeDropDown->GetSelectedOption();
	TEnumAsByte<EWindowMode::Type> windowMode;
	if (GetWindowMode(windowModeName, windowMode))
	{
		userSettings->SetFullscreenMode(windowMode);
	}

	userSettings->ApplySettings(false);
}

void UMainMenu::BeginConstruct()
{
	//child class override this for native construct
}

bool UMainMenu::GetResolutionSetting(const FName& name, FIntPoint& outRes) const
{
	auto res = ResolutionSettings.Find(name);
	if (res)
	{
		outRes = *res;
		return true;
	}
	return false;
}

bool UMainMenu::GetWindowMode(const FName& name, TEnumAsByte<EWindowMode::Type>& outMode) const
{
	auto mode = windowModeSettings.Find(name);
	if (mode)
	{
		outMode = *mode;
		return true;
	}
	return false;
}

FName UMainMenu::GetNameForRes(const FIntPoint& res) const
{
	return GetKeyFromMap(ResolutionSettings, res);
}

FName UMainMenu::GetNameForFullscreenMode(TEnumAsByte<EWindowMode::Type> type) const
{
	return GetKeyFromMap(windowModeSettings, type);
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	userSettings = UGameUserSettings::GetGameUserSettings();
	if (userSettings)
	{
		PullGameSettingToDropdownList();
		ApplyGameSettings();
	}
	BeginConstruct();
}

void UMainMenu::PullGameSettingToDropdownList()
{
	auto ScreenRes = userSettings->GetScreenResolution();
	auto windowMode = userSettings->GetFullscreenMode();

	FName screenResOption = GetNameForRes(ScreenRes);
	FName windowModeOption = GetNameForFullscreenMode(windowMode);
	ResolutionDropdown->SetSelectedOption(screenResOption);
	ScreenModeDropDown->SetSelectedOption(windowModeOption);
}
