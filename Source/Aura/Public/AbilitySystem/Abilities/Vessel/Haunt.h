// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Haunt.generated.h"

class AHauntProjectile;

/**
 * 
 */
UCLASS()
class AURA_API UHaunt : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	// Disable and hide DemonicSoul (since we still need access to their attributes later), spawn a cosmetic version of AHauntProjectile, which will travel back to the Vessel via blueprint timeline
	UFUNCTION(BlueprintCallable)
	void SpawnReturnProjectile(); 

	// Spawn a fully function HauntProjectile, explodes on hit dealing damage and "spawning" the Demonic Soul back into the game
	UFUNCTION(BlueprintCallable)
	void FireDemonicSoul(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> HauntPassiveEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ReturnSound;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHauntProjectile> HauntProjectileClass;
};
