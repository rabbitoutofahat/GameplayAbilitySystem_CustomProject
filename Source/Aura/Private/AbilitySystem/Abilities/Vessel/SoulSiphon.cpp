// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SoulSiphon.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/EnemyInterface.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

void USoulSiphon::ApplyDamageToTarget(AActor* ActorToDamage)
{
	if (!IsValid(ActorToDamage) || !ActorToDamage->Implements<UEnemyInterface>()) return;
	CauseDamage(ActorToDamage);
}

// Spawn soul orbs with different initial trajectories that will travel from the target to the ability owner (travel path handled in blueprint)
void USoulSiphon::SpawnSoulOrbsAtTarget(AActor* DamagedActor, int32 NumOrbs)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FRotator Rotation = (GetAvatarActorFromActorInfo()->GetActorLocation() - DamagedActor->GetActorLocation()).Rotation(); // Want to spawn orbs facing towards the ability owner
	TArray<FRotator> SpawnRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Rotation.Vector(), FVector::UpVector, OrbSpread, NumOrbs); // Always spawn 3 orbs on primary target, 1 otherwise

	for (const FRotator& SpawnRotation : SpawnRotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(DamagedActor->GetActorLocation() + (Rotation.Vector() * OrbSpawnDistance));
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		AAuraProjectile* SoulOrb = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			SoulOrbClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		AActor* HomingTarget = GetAvatarActorFromActorInfo();
		SoulOrb->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		SoulOrb->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		SoulOrb->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		SoulOrb->FinishSpawning(SpawnTransform);
	}
}
