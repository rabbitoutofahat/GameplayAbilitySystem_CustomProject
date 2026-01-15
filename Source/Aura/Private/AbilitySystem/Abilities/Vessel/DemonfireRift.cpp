// Roland Frances


#include "AbilitySystem/Abilities/Vessel/DemonfireRift.h"
#include "Actor/ProjectileSpawner.h"

void UDemonfireRift::SpawnRift()
{
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Left = Forward.RotateAngleAxis(-90.f, FVector::UpVector);
	const float RandomSpread = FMath::RandRange(0.f, 180.f);

	const FVector SpawnDirection = Left.RotateAngleAxis(RandomSpread, FVector::UpVector);
	FVector SpawnLocation = Location + SpawnDirection * FMath::FRandRange(RiftSpawnDistanceMin, RiftSpawnDistanceMax);

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	AProjectileSpawner* Rift = GetWorld()->SpawnActorDeferred<AProjectileSpawner>(
		RiftClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Rift->Owner = GetAvatarActorFromActorInfo();
	Rift->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Rift->SetActorEnableCollision(false);
	Rift->FinishSpawning(SpawnTransform);
}
