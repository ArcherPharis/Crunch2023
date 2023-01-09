// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActor_GroundPick.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTargetActor.h"

AGATargetActor_GroundPick::AGATargetActor_GroundPick()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);
	
	TargetArea = CreateDefaultSubobject<USphereComponent>("TargetArea");
	TargetArea->SetupAttachment(RootComp);

	TargetArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	PrimaryActorTick.bCanEverTick = true;
}

void AGATargetActor_GroundPick::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}

void AGATargetActor_GroundPick::ConfirmTargetingAndContinue()
{
	TArray<AActor*> overlappyingActors;
	TargetArea->GetOverlappingActors(overlappyingActors);
	overlappyingActors.Remove(OwningAbility->GetAvatarActorFromActorInfo());
	
	UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(overlappyingActors, false);

	//specify a location info, can't use sharedptr because if we do, the later in TargetDataWithLocInfoAndActors.Add(locInfo); another shared ptr need to be created with the raw data again internally, which will have 2 set of sharedptrs tracking the same asset.
	FGameplayAbilityTargetData_LocationInfo* locInfo = new FGameplayAbilityTargetData_LocationInfo;
	locInfo->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
	locInfo->TargetLocation.LiteralTransform = GetActorTransform();

	//specify a target info, this is an example to use shapred ptr, but later on, we need to pass over to TargetDataWithLocInfoAndActors with a shared ptr constructor.
	TSharedPtr<FGameplayAbilityTargetData_ActorArray> targetedActorInfo{ new FGameplayAbilityTargetData_ActorArray };
	for (auto actor : overlappyingActors)
	{
		if (IsActorHostileToAbilityOwner(actor))
		{
			targetedActorInfo->TargetActorArray.Add(actor);
		}
	}

	//add the two target datas
	FGameplayAbilityTargetDataHandle TargetDataWithLocInfoAndActors;
	TargetDataWithLocInfoAndActors.Add(locInfo);
	TargetDataWithLocInfoAndActors.Data.Add(targetedActorInfo);

	TargetDataReadyDelegate.Broadcast(TargetDataWithLocInfoAndActors);
}

void AGATargetActor_GroundPick::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (MasterPC)
	{
		FVector Loc;
		FRotator Rot;
		MasterPC->GetActorEyesViewPoint(Loc, Rot);

		FVector MasterLookDir = UKismetMathLibrary::GetForwardVector(Rot);

		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, Loc, Loc + MasterLookDir * TNumericLimits<float>::Max(), ECollisionChannel::ECC_Visibility))
		{
			SetActorLocation(hitResult.Location);
		}
	}
}
