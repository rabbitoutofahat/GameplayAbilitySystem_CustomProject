// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Upgrades/DemonfireRift/DemonfireRift_RiftMastery.h"

FString UDemonfireRift_RiftMastery::GetDescription(int32 Level)
{
	return FString::Printf(TEXT(
		// Title
		"<Title>Rift Mastery</>\n"

		// Description
		"<Default>Demonfire Rifts fire an additional %d Demonfire Bolts over its duration</>"),
		AdditionalBolts);
}

FString UDemonfireRift_RiftMastery::GetNextLevelDescription(int32 Level)
{
	return FString();
}
