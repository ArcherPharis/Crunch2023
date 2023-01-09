// Fill out your copyright notice in the Description page of Project Settings.


#include "HitDetectionComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GenericTeamAgentInterface.h"
UHitDetectionComponent::UHitDetectionComponent()
{
	OnComponentBeginOverlap.AddDynamic(this, &UHitDetectionComponent::Overlapped);
}

void UHitDetectionComponent::Overlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
		return;

	IGenericTeamAgentInterface* OwnerInterface = Cast<IGenericTeamAgentInterface>(GetOwner());
	if (OwnerInterface)
	{
		if (OwnerInterface->GetTeamAttitudeTowards(*OtherActor) == ETeamAttitude::Friendly)
		{
			return;
		}
	}

	FGameplayEventData eventData;

	FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherActor);
	eventData.TargetData = TargetData;
	eventData.EventMagnitude = AttackEffectMagnitude;
	eventData.ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
	FHitResult hitResult = FHitResult{ GetOwner(), this, GetComponentLocation(), GetComponentVelocity().GetSafeNormal() };
	hitResult.Distance = AttackEffectMagnitude;
	eventData.ContextHandle.AddHitResult(hitResult);
	eventData.ContextHandle.AddInstigator(GetOwner(), GetOwner());
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), HitEventTag, eventData);
}
