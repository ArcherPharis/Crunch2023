// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "CTAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "GenericTeamAgentInterface.h"
#include "CTCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityGiven, const FGameplayAbilitySpec*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeathStarted);


UCLASS()
class CRUNCHTIME_API ACTCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	ACTCharacterBase();
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	
	/**************************************************************************************/
	/*                                       TEAM                                         */
	/**************************************************************************************/
public:
	/** Assigns Team Agent to given TeamID */
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& ID) { TeamID = ID; }

	/** Retrieve team identifier in form of FGenericTeamId */
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const { return TeamID; }
private:
	UPROPERTY(EditAnywhere, Category = "AI")
	FGenericTeamId TeamID;


	/**************************************************************************************/
	/*                                    Gameplay Ability                                */
	/**************************************************************************************/
public:
	/*Used by AI to find an ability*/
	TSubclassOf<UGameplayAbility> GetAbilityByIndex(int index) const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	FORCEINLINE class UCTAttributeSet* GetAttributeSet() const { return AttributeSet; }
private:
	UPROPERTY()
	class UCTAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY()
	class UCTAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TMap<ECTAbilityInputID, TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//                         - Initialization -                          //
public:
	void ApplayInitialEffect();
	void ApplyRegenEffect();
	FOnAbilityGiven OnAbilityGiven;

private:
	FGameplayAbilitySpec* GiveAbility(const TSubclassOf<class UGameplayAbility>& newAbility, int inputID = -1, bool broadCast = true);

	//                         - Gameplay Effects -                        //
protected:
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	void ApplyEffectToSelf(const TSubclassOf<class UGameplayEffect>& effectToApply);

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TSubclassOf<class UGameplayEffect> InitialEffect;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> RegenEffects;

	//                         - Abilitiy montages -                        //
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbility")
	UAnimMontage* MeleeHitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbility")
	UAnimMontage* LaunchHitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbility")
	UAnimMontage* KnowDownHitMontage;



	//                         - Basic Attack -                            //
public:
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	void DoBasicAttack();

protected:
	void BasicAttack();

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	TSubclassOf<class UGameplayAbility> BasicAttackAbility;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	FGameplayTag BasicAttackCombo;

	UPROPERTY(VisibleDefaultsOnly, Category = "GameplayAbility")
	class UHitDetectionComponent* HitDetectionComp;

	/**************************************************************************************/
	/*                                   Aiming                                           */
	/**************************************************************************************/
public:
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	void SetEanbleAiming(bool bEnableMoving);
	
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	FORCEINLINE	bool GetIsAiming() const { return bIsAiming; }
private:
	bool bIsAiming;


	/**************************************************************************************/
	/*                                   Gameplay Events                                  */
	/**************************************************************************************/
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameplayAbility", meta=(DisplayName = "HealthUpdated"))
	void BP_HealthUpdated(float Health, float HealthDelta, float maxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameplayAbility", meta = (DisplayName = "OnDeath"))
	void BP_OnDeath();

private:
	void HealthChanged(const FOnAttributeChangeData& AttributeData);


	/**************************************************************************************/
	/*                                   Death                                            */
	/**************************************************************************************/
public:
		FOnCharacterDeath OnCharacterDeath;
		FOnCharacterDeathStarted OnCharacterDeathStarted;

private:
	void StartDeathSequence();
	
	void OnDeath();
	
	bool bIsDead = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	TSubclassOf<UGameplayEffect> DeathEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	float DeathDelayAfterMontage = 1.f;


	/**************************************************************************************/
	/*                                    Stun & Disabling                                */
	/**************************************************************************************/
private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	FGameplayTag StunTag;
	void StunTagChanged(const FGameplayTag tag, int32 count);

	void DisablePlayerInput();
	void EnablePlayerInput();
	
	void Disable();
	void RecoverFromDisable();

	void StopAllAbilitites();


	/**************************************************************************************/
	/*                                       AI                                           */
	/**************************************************************************************/
private:
	void PauseAILogic(const FString& Reason = "Generic");
	void ResumeAILogic(const FString& Reason = "Generic");

	//                             - Perception -                               //
	void DisableAIRelavency();
	UPROPERTY()
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliComp;
};
