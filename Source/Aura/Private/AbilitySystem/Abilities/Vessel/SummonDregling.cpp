// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"
#include "Actor/AuraProjectile.h"

void USummonDregling::SpawnDreglingProjectile(const FVector& TargetLocation, const float XOffset, const float YOffset)
{
	const FVector SpawnLocation = TargetLocation + FVector(XOffset, YOffset, SpawnHeight);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation((SpawnLocation - TargetLocation).ToOrientationQuat());
	AAuraProjectile* DreglingProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		DreglingProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	DreglingProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	DreglingProjectile->FinishSpawning(SpawnTransform);
}