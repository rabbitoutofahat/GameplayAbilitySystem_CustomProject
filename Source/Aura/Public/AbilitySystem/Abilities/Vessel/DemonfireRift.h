// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "DemonfireRift.generated.h"

class AProjectileSpawner;

/**
 * 
 */
UCLASS()
class AURA_API UDemonfireRift : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void SpawnRift();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileSpawner> RiftClass;

	UPROPERTY(EditDefaultsOnly)
	float RiftSpawnDistanceMin = 150.f;

	UPROPERTY(EditDefaultsOnly)
	float RiftSpawnDistanceMax = 400.f;
};
