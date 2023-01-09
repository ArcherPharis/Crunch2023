// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class ECTAbilityInputID : uint8
{
	NONE,
	Confirm,
	Cancel,
	AbilityOne,
	AbilityTwo,
	AbilityThree,
	AbilityFour,
	AbilityFive
};