// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "DreglingExplosion.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDreglingExplosion : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void StartDissolve();

	UFUNCTION(BlueprintCallable)
	void Explosion();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ExplosionTimer;
};
