// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "DemonfireRift.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDemonfireRift : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RiftDuration;
};
