// Roland Frances


#include "Actor/ProjectileSpawner.h"
#include "Actor/AuraProjectile.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileSpawner::AProjectileSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

TArray<AAuraEnemy*> AProjectileSpawner::GetNearbyEnemies()
{
	TArray<AActor*> OutOverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, OutOverlappingActors, ActorsToIgnore, EffectRadius, GetActorLocation());

	TArray<AAuraEnemy*> ValidTargets;
	for (AActor* Actor : OutOverlappingActors)
	{
		if (AAuraEnemy* Enemy = Cast<AAuraEnemy>(Actor)) ValidTargets.Add(Enemy);
	}
	return ValidTargets;
}

void AProjectileSpawner::SpawnProjectileAtRandomTarget(const TArray<AAuraEnemy*>& ValidTargets)
{
	if (ValidTargets.Num() == 0) return;
	AAuraEnemy* ProjectileTarget = ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];
	if (!ProjectileTarget) { return; }

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());
	FRotator Rotation = (ProjectileTarget->GetActorLocation() - GetActorLocation()).Rotation();
	Rotation.Pitch = ProjectilePitch;
	SpawnTransform.SetRotation(Rotation.Quaternion());
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwner(),
		Cast<APawn>(GetOwner()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = DamageEffectParams;
	Projectile->ProjectileMovement->HomingTargetComponent = ProjectileTarget->GetRootComponent();
	Projectile->FinishSpawning(SpawnTransform);
}


