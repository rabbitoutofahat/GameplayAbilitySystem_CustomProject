// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"
#include "Actor/AuraProjectile.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystemComponent.h"

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

	if (Cast<AVessel>(GetAvatarActorFromActorInfo())->GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling_KineticBombardment))
	{
		for (int32 i = 0; i < KineticBombardmentNumProjectiles; i++)
		{
			float RandomProjectileSpread = FMath::RandRange(-ProjectileSpread, ProjectileSpread);
			FTransform KineticBombardmentTransform;
			KineticBombardmentTransform.SetLocation(SpawnLocation);
			KineticBombardmentTransform.SetRotation((TargetLocation - SpawnLocation + FVector(RandomProjectileSpread, RandomProjectileSpread, 0.f)).ToOrientationQuat());
		
			AAuraProjectile* KineticBombardmentProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				KineticBombardmentProjectileClass,
				KineticBombardmentTransform,
				GetAvatarActorFromActorInfo(),
				CurrentActorInfo->PlayerController->GetPawn(),
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			KineticBombardmentProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
			KineticBombardmentProjectile->FinishSpawning(KineticBombardmentTransform);
		}
	}
}