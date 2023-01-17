// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CTGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UCTGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	class ACTCharacterBase* GetAvatarAsCTCharacter() const;

	void SetEnableAim(bool bAimMode);
	void PushActorAsCharacter(AActor* actorToPush,const FVector& PushDir, float PushStrength) const;
	void PushActorsInTargetData(const FGameplayAbilityTargetDataHandle& tagetData, const FVector& PushDir, float PushStrength) const;

	UPROPERTY(EditDefaultsOnly, Category = "Generic Effects")
	TSubclassOf<UGameplayEffect> StunEffect;

	void ApplyStunEffectToTarget(FGameplayAbilityTargetDataHandle TargetData);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
public:
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool IsOtherHostile(const AActor* otherActor) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	UTexture2D* AbilityIcon;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetCooldownDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	float GetStaimiaCost() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	int GetAssignedLevel(UAbilitySystemComponent* owningComp) const;

private:
	ACTCharacterBase* AvatarCTCharacterBase;
};
