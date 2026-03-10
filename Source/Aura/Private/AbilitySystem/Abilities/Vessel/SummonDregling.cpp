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

	AVessel* Vessel = Cast<AVessel>(GetAvatarActorFromActorInfo());
	if (Vessel->GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling_KineticBombardment) && Vessel->bKineticBombardmentOffCD)
	{
		for (int32 i = 0; i < KineticBombardmentNumProjectiles; i++)
		{
			// Add deviation from main Dregling Projectile's target location
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

		Vessel->bKineticBombardmentOffCD = false;
		FTimerDelegate KineticBombardmentDelegate;
		KineticBombardmentDelegate.BindLambda([this, Vessel]()
			{
				GetWorld()->GetTimerManager().ClearTimer(Vessel->KineticBombardmentTimerHandle);
				Vessel->bKineticBombardmentOffCD = true;
			});
		GetWorld()->GetTimerManager().SetTimer(Vessel->KineticBombardmentTimerHandle, KineticBombardmentDelegate, KineticBombardmentCooldown, false);
	}
}