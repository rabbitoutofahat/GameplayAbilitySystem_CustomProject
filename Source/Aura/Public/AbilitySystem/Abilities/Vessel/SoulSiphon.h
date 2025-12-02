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

	UFUNCTION(BlueprintCallable)
	void SpawnSoulOrbsAtTarget(AActor* DamagedActor, int32 NumOrbs);

private:
	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	TSubclassOf<AAuraProjectile> SoulOrbClass;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float OrbSpawnDistance = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float OrbSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "SoulOrb")
	bool bLaunchHomingProjectiles = true;
};

