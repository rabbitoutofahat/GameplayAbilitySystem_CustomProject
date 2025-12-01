// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "SoulSiphon.generated.h"

class UNiagaraSystem;

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
	TArray<UNiagaraComponent*> SpawnSoulOrbsAtTarget(AActor* DamagedActor);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Soul Orb")
	TObjectPtr<UNiagaraSystem> SoulOrbEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Soul Orb")
	float OrbSpawnDistance = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Soul Orb")
	float OrbSpread = 90.f;
};

