// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonInfernalGolem.h"
#include "Character/PlayableClasses/Vessel.h"

void USummonInfernalGolem::SummonInfernalGolem(const FTransform& TargetLocation)
{
	if (AVessel* Vessel = Cast<AVessel>(GetAvatarActorFromActorInfo()))
	{
		Vessel->SpawnSummonedMinion(GolemClass, TargetLocation, Vessel->GetPlayerLevel_Implementation());
	}
}