// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActor_Frontal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
AGATargetActor_Frontal::AGATargetActor_Frontal()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);

	TargetRange = CreateDefaultSubobject<UBoxComponent>("TargetRange");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AGATargetActor_Frontal::ConfirmTargetingAndContinue()
{
	TArray<AActor*> overlappyingActors;
	TargetRange->GetOverlappingActors(overlappyingActors);
	overlappyingActors.RemoveAll([&](AActor* actor){return !IsActorHostileToAbilityOwner(actor); });

	overlappyingActors.Remove(Owner);
	FGameplayAbilityTargetDataHandle data = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(overlappyingActors, false);
	TargetDataReadyDelegate.Broadcast(data);
}
