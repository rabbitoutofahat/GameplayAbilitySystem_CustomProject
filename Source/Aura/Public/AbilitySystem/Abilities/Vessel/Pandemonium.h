// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Pandemonium.generated.h"

/**
 * Ability allows all summoned minions to use their special ability once free-of-charge
 */
UCLASS()
class AURA_API UPandemonium : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EffectRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> NoCostGameplayEffect;
};
