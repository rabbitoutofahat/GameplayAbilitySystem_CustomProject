// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilityTypes.h"
#include "ProjectileSpawner.generated.h"

class AAuraProjectile;
class AAuraEnemy;

UCLASS()
class AURA_API AProjectileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileSpawner();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	UFUNCTION(BlueprintCallable)
	TArray<AAuraEnemy*> GetNearbyEnemies();

	UFUNCTION(BlueprintCallable)
	void SpawnProjectileAtRandomTarget(const TArray<AAuraEnemy*>& ValidTargets);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnDuration = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileSpread = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float EffectRadius = 800.f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectilePitch = 45.f;
};
