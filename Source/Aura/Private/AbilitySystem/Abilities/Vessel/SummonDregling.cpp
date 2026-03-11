// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"
#include "Actor/AuraProjectile.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

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
		SpawnKineticBombardmentProjectiles(SpawnLocation, TargetLocation);
		SetKineticBombardmentActiveStatus(Vessel, false);
		FTimerDelegate KineticBombardmentDelegate;
		KineticBombardmentDelegate.BindLambda([this, Vessel]()
			{
				GetWorld()->GetTimerManager().ClearTimer(Vessel->KineticBombardmentTimerHandle);
				SetKineticBombardmentActiveStatus(Vessel, true);
			});
		GetWorld()->GetTimerManager().SetTimer(Vessel->KineticBombardmentTimerHandle, KineticBombardmentDelegate, Cooldown, false);
	}
}

void USummonDregling::SpawnKineticBombardmentProjectiles(const FVector& SpawnLocation, const FVector& TargetLocation)
{
	for (int32 i = 0; i < NumProjectiles; i++)
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
}

void USummonDregling::SetKineticBombardmentActiveStatus(AVessel* Vessel, bool bInStatus)
{
	Vessel->bKineticBombardmentOffCD = bInStatus;
	Cast<UAuraAbilitySystemComponent>(Vessel->GetAbilitySystemComponent())->SpellGlobeLightUp.Broadcast(bInStatus, FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling);
}