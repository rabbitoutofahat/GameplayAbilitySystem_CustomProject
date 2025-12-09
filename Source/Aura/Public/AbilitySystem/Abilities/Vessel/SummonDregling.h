// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "SummonDregling.generated.h"

class AAuraProjectile;
class ASummonCharacter;

/**
 * 
 */
UCLASS()
class AURA_API USummonDregling : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	// Summons a Dregling Summon Character from the sky that crashes down onto the TargetLocation. Should feel like a meteor strike, with XY-offsets to have it come down at an angle.
	UFUNCTION(BlueprintCallable)
	void SpawnDreglingProjectile(const FVector& TargetLocation, const float XOffset, const float YOffset);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnHeight;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> DreglingProjectileClass;
};
