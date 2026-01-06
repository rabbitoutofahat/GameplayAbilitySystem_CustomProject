// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonChaosElemental.h"
#include "Character/PlayableClasses/Vessel.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"
#include "BrainComponent.h"

void USummonChaosElemental::SummonChaosElemental(const FTransform& TargetLocation)
{
	AVessel* Vessel = Cast<AVessel>(GetAvatarActorFromActorInfo());
	if (Vessel == nullptr) return;
	ASummonCharacter* ChaosEle = Vessel->SpawnSummonedMinion(ElementalClass, TargetLocation, Vessel->GetPlayerLevel_Implementation());

	FGameplayCueParameters CueParams;
	CueParams.Location = ChaosEle->GetActorLocation();
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(ChaosEle, FAuraGameplayTags::Get().GameplayCue_Rift_ChaosElemental, CueParams);

	Cast<AAuraAIController>(ChaosEle->GetController())->GetBrainComponent()->StopLogic("Start Spawning Actor");
	ChaosEle->bIsBeingSpawned = true;
}