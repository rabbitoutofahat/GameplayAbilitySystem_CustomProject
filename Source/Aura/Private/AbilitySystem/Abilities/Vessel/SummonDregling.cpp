// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"
#include "Actor/AuraProjectile.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"

void USummonDregling::SpawnDreglingProjectile(const FVector& TargetLocation, const float XOffset, const float YOffset)
{
	const FVector SpawnLocation = TargetLocation + FVector(XOffset, YOffset, SpawnHeight);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation((TargetLocation - SpawnLocation).ToOrientationQuat());
	AAuraProjectile* DreglingProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		DreglingProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	FGameplayCueParameters CueParams;
	CueParams.Location = SpawnLocation;
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(DreglingProjectile, FAuraGameplayTags::Get().GameplayCue_Rift_Dregling, CueParams);

	DreglingProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	DreglingProjectile->FinishSpawning(SpawnTransform);
}