// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpec.h"
#include "CTPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API ACTPlayerController : public APlayerController
{
	GENERATED_BODY()

	/**************************************************************************************/
	/*                                        UI                                          */
	/**************************************************************************************/
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FORCEINLINE class UInGameUI* GetInGameUI() const { return inGameUI; }

	void UpdateAbilityIcon(UGameplayAbility* Ability);
	void LevelUpUI();

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UInGameUI> InGameUIClass;
	UInGameUI* inGameUI;
	UFUNCTION()
	void TogglePauseMenu();
	void ToggleShop();

	void SetPaused(bool bPaused);

	/**************************************************************************************/
	/*                                        Pawn                                        */
	/**************************************************************************************/
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	class ACTCrunch* Crunch;
	/* - stats - */
	void HealthUpdated(const FOnAttributeChangeData& AttributeData);
	void StaminaUpdated(const FOnAttributeChangeData& AttributeData);
	void StengthUpdated(const FOnAttributeChangeData& AttributeData);
	void AttackDamageUpdated(const FOnAttributeChangeData& AttributeData);
	void IntelligenceUpdated(const FOnAttributeChangeData& AttributeData);
	void ArmorUpdated(const FOnAttributeChangeData& AttributeData);
	void WalkSpeedUpdated(const FOnAttributeChangeData& AttributeData);
	void LevelUpdated(const FOnAttributeChangeData& AttributeData);
	void ExperienceUpdated(const FOnAttributeChangeData& AttributeData);
	void NextLevelExperienceUpdated(const FOnAttributeChangeData& AttributeData);
	void CreditsUpdated(const FOnAttributeChangeData& AttributeData);



	/* - Pawn Ability - */
	void PawnAddAbility(const FGameplayAbilitySpec* SpecHandle);

	void AbilityActivated(UGameplayAbility* ability);

	/* - Pawn Death - */
	UFUNCTION()
	void PawnDeathStarted();
	UFUNCTION()
	void PawnDead();
	UFUNCTION()
	void PawnTookDamage();

	bool bIsPawnDead{false};

	/**************************************************************************************/
	/*                                        Input                                       */
	/**************************************************************************************/
private:
	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
};
