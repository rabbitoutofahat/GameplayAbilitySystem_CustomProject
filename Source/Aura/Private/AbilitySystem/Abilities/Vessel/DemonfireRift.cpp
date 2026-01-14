// Roland Frances


#include "AbilitySystem/Abilities/Vessel/DemonfireRift.h"

FVector UDemonfireRift::GetRandomRiftSpawnLocation() const
{
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Left = Forward.RotateAngleAxis(-90.f, FVector::UpVector);
	const float RandomSpread = FMath::RandRange(0.f, 180.f);

	const FVector SpawnDirection = Left.RotateAngleAxis(RandomSpread, FVector::UpVector);
	FVector SpawnLocation = Location + SpawnDirection * FMath::FRandRange(RiftSpawnDistanceMin, RiftSpawnDistanceMax);

	return SpawnLocation;
}
