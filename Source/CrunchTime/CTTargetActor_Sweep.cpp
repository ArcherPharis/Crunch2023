// Fill out your copyright notice in the Description page of Project Settings.


#include "CTTargetActor_Sweep.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ACTTargetActor_Sweep::ACTTargetActor_Sweep()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);

	SweepTargetArea = CreateDefaultSubobject<UCapsuleComponent>("SweepTargetArea");
	SweepTargetArea->SetupAttachment(GetRootComponent());

	SweepTargetArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	SweepTargetArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SweepTargetArea->OnComponentBeginOverlap.AddDynamic(this, &ACTTargetActor_Sweep::TargetAreaOverlapped);
}

void ACTTargetActor_Sweep::TargetAreaOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActorHostileToAbilityOwner(OtherActor)) return;

	auto targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherActor);
	TargetDataReadyDelegate.Broadcast(targetData);
}
