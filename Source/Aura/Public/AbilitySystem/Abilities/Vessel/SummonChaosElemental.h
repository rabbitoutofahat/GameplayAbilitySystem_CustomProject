// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "SummonChaosElemental.generated.h"

class ASummonCharacter;

/**
 * 
 */
UCLASS()
class AURA_API USummonChaosElemental : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void SummonChaosElemental(const FTransform& TargetLocation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASummonCharacter> ElementalClass;
};
