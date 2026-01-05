// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "SummonInfernalGolem.generated.h"

class ASummonCharacter;

/**
 * 
 */
UCLASS()
class AURA_API USummonInfernalGolem : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void SummonInfernalGolem(const FTransform& TargetLocation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASummonCharacter> GolemClass;
};
