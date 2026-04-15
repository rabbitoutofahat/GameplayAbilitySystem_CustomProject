// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AbilityUpgrade.h"
#include "Pandemonium_Uproar.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPandemonium_Uproar : public UAbilityUpgrade
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription() override;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

private:
	int32 NumCharges = 1; // Stored here for description (updated when upgrade is applied)
};
