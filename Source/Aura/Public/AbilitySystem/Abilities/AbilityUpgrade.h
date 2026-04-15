// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAbilityUpgrade : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription();
};
