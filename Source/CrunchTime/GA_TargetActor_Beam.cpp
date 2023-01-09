// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_TargetActor_Beam.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AGA_TargetActor_Beam::AGA_TargetActor_Beam()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	SetRootComponent(RootComp);
	
	BeamBoom = CreateDefaultSubobject<USpringArmComponent>("BeamBoom");
	BeamBoom->SetupAttachment(GetRootComponent());

	TargetArea = CreateDefaultSubobject<UBoxComponent>("TargetArea");
	TargetArea->SetupAttachment(BeamBoom, USpringArmComponent::SocketName);

	PrimaryActorTick.bCanEverTick = true;

	TargetArea->OnComponentBeginOverlap.AddDynamic(this, &AGA_TargetActor_Beam::TargetAreaOverlapped);
	TargetArea->OnComponentEndOverlap.AddDynamic(this, &AGA_TargetActor_Beam::TargetAreaEndOverlap);
	TargetArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

FVector AGA_TargetActor_Beam::GetEndLoc() const
{
	return TargetArea->GetComponentLocation();
}

float AGA_TargetActor_Beam::GetBeamLength() const
{
	return (BeamBoom->GetComponentLocation() - TargetArea->GetComponentLocation()).Length();
}

void AGA_TargetActor_Beam::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(timerHandle, this, &AGA_TargetActor_Beam::ReportTargets, 1/targetReportFrequency, true);
}

void AGA_TargetActor_Beam::TargetAreaOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != OwningAbility->GetAvatarActorFromActorInfo() && IsActorHostileToAbilityOwner(OtherActor))
	{
		CurrentTargets.AddUnique(OtherActor);
	}
}

void AGA_TargetActor_Beam::TargetAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != OwningAbility->GetAvatarActorFromActorInfo() && IsActorHostileToAbilityOwner(OtherActor))
	{
		CurrentTargets.Remove(OtherActor);
	}
}

void AGA_TargetActor_Beam::ReportTargets()
{
	FGameplayAbilityTargetData_ActorArray* actorArray = new FGameplayAbilityTargetData_ActorArray;
	actorArray->TargetActorArray.Reset();
	for (AActor* targetActor : CurrentTargets)
	{
		actorArray->TargetActorArray.Add(targetActor);
	}

	FGameplayAbilityTargetDataHandle targetData{ actorArray };


	TargetDataReadyDelegate.Broadcast(targetData);
}
