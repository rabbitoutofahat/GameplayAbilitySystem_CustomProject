// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonInfernalGolem.h"
#include "Character/PlayableClasses/Vessel.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"
#include "BrainComponent.h"

void USummonInfernalGolem::SummonInfernalGolem(const FTransform& TargetLocation)
{
	AVessel* Vessel = Cast<AVessel>(GetAvatarActorFromActorInfo());
	if (Vessel == nullptr) return;
	ASummonCharacter* Golem = Vessel->SpawnSummonedMinion(GolemClass, TargetLocation, Vessel->GetPlayerLevel_Implementation());
	
	FGameplayCueParameters CueParams;
	CueParams.Location = Golem->GetActorLocation();
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(Golem, FAuraGameplayTags::Get().GameplayCue_Rift_InfernalGolem, CueParams);
	
	Cast<AAuraAIController>(Golem->GetController())->GetBrainComponent()->StopLogic("Start Spawning Actor");
	Golem->bIsBeingSpawned = true;
}