// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Pandemonium.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPandemonium : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void CastPandemonium(); // Ability allows all summoned minions to use their special ability once free-of-charge
	
	UPROPERTY(EditDefaultsOnly)
	float EffectRadius = 1000.f;
};
