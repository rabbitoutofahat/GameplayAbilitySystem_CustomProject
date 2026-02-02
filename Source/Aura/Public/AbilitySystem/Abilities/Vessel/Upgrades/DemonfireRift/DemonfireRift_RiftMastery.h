// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Abilities/Vessel/DemonfireRift.h"
#include "DemonfireRift_RiftMastery.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDemonfireRift_RiftMastery : public UDemonfireRift
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
