// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "DemonfireRift.generated.h"

class AAuraProjectile;

/**
 * 
 */
UCLASS()
class AURA_API UDemonfireRift : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	FVector GetRandomRiftSpawnLocation() const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> RiftProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RiftDuration = 8.f;

	UPROPERTY(EditDefaultsOnly)
	float RiftSpawnDistanceMin = 150.f;

	UPROPERTY(EditDefaultsOnly)
	float RiftSpawnDistanceMax = 400.f;
};
