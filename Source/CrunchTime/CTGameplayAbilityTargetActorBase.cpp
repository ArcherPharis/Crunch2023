// Fill out your copyright notice in the Description page of Project Settings.


#include "CTGameplayAbilityTargetActorBase.h"
#include "Abilities/GameplayAbility.h"
#include "GenericTeamAgentInterface.h"

bool ACTGameplayAbilityTargetActorBase::IsActorHostileToAbilityOwner(AActor* otherActor) const
{
	if (OwningAbility)
	{
		IGenericTeamAgentInterface* AbilityOwnerAIInterface =Cast<IGenericTeamAgentInterface>(OwningAbility->GetAvatarActorFromActorInfo());
		if (AbilityOwnerAIInterface && otherActor && AbilityOwnerAIInterface->GetTeamAttitudeTowards(*otherActor) == ETeamAttitude::Hostile)
		{
			return true;
		}
	}
	return false;
}