// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "SoulSiphon.generated.h"

class AAuraProjectile;

/**
 * 
 */
UCLASS()
class AURA_API USoulSiphon : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget(AActor* ActorToDamage);


private:
	void SpawnSoulOrbsAtTarget(AActor* DamagedActor, int32 NumOrbs);

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	TSubclassOf<AAuraProjectile> SoulOrbClass;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float OrbSpawnDistance = 5.f; // Push the initial spawn location a little bit away from the target's mesh, purely cosmetic

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float OrbSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float HomingAccelerationMin = 6400.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float HomingAccelerationMax = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	bool bLaunchHomingProjectiles = true;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	float SoulCrushInnerRadius = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	float SoulCrushOuterRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	TSubclassOf<UNiagaraSystem> SoulCrushEffect;
};

