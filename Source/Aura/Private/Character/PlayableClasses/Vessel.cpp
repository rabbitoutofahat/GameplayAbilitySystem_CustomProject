// Roland Frances


#include "Character/PlayableClasses/Vessel.h"

ASummonCharacter* AVessel::SpawnSummonedMinion(UClass* Class, const FTransform& SpawnTransform, int32 OwnerLevel)
{
	ASummonCharacter* Summon = GetWorld()->SpawnActorDeferred<ASummonCharacter>(Class, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Summon->OwnerActor = this;
	Summon->SetLevel(OwnerLevel); // Set after SpawnSummonedMinion is called in blueprint
	Summon->FinishSpawning(SpawnTransform);
	Summon->SpawnDefaultController();
	return Summon;
}