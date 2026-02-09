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
	/*
	*  Apply damage to the target actor and spawn soul orbs 
	*  If the Soul Crush upgrade is equipped, deal radial damage to enemies around the target as well and spawn an additional soul orb per enemy hit
	*/
	UFUNCTION(BlueprintCallable)
	void SoulSiphon(AActor* TargetActor);

	/*
	* Soul Siphon upgrade that refunds a proportion of the cost of summoning a given SummonCharacter back to the player as souls/mana upon that SummonCharacter's death
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	float ReclamationPercentage = 0.4f;

private:
	/*
	*  Spawn soul orbs with different initial trajectories that will travel from the target to the ability owner (travel path handled in blueprint)
	*/
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

	// Soul Crush Upgrade - Changes Soul Siphon from single target to radial damage around the target
	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	float SoulCrushInnerRadius = 40.f;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	float SoulCrushOuterRadius = 200.f;

	// Vengeful Manifestation Upgrade - Chance upon collecting a soul orb to spawn a Vengeful Spirit to fight you
	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	float VengefulSpiritSpawnPercent = 30.f;
};

