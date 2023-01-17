// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayAbilitySpec.h"
#include "InGameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumeBtnClicked);
/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnResumeBtnClicked OnResumeBtnClicked;

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateHealth(float newHealth, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateStamina(float newVal, float maxVal);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateStrength(int newVal);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateLevel(int newVal);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateExperience(int newVal);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateNextLevelExperience(int newVal);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateAttackPower(int newVal);
	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateIntelligence(int newVal);
	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateArmor(int newVal);
	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void UpdateWalkSpeed(int newVal);




	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void AbilityAdded(UGameplayAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void AbilityActivated(UGameplayAbility* Ability);

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void SwitchToPauseUI();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void SwitchToGameplayUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void SwitchToDeathUI();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "InGameUI")
	void DeathStarted();

	void ChangeHeadshotImage();
	void HeadshotDamageReact();


protected:
	UFUNCTION(BlueprintCallable, Category = "InGameUI")
	void ResumeBtnClicked();

	UFUNCTION(BlueprintCallable, Category = "InGameUI")
	void BackToMainMenuBtnClicked();

	UFUNCTION(BlueprintCallable, Category = "InGameUI")
	void QuitGameBtnClicked();

	UFUNCTION(BlueprintCallable, Category = "InGameUI")
	void RestartBtnClicked();

	virtual void NativeConstruct() override;

	class ACrunchTimeGameModeBase* GameMode;

private:
	UPROPERTY(meta = (BindWidget))
	class UHeadshot* headShotImage;
};
