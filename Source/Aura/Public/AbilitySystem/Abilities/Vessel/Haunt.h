// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Haunt.generated.h"

class ASummonCharacter;
class AHauntProjectile;

/**
 * 
 */
UCLASS()
class AURA_API UHaunt : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void HideSummon(ASummonCharacter* SummonClass, bool Enable);
	
	// Disable and hide DemonicSoul (since we still need access to their attributes later), spawn a cosmetic version of AHauntProjectile, which will travel back to the Vessel via blueprint timeline
	UFUNCTION(BlueprintCallable)
	void SpawnReturnProjectile(); 

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHauntProjectile> HauntProjectileClass;
};
