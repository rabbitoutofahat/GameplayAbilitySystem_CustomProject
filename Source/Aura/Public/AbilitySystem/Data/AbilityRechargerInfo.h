// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityRechargerInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;
struct FGameplayAbilitySpec;

USTRUCT(BlueprintType)
struct FRechargerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityRecharger= nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> RechargeEffect = nullptr;
};

/**
 * We associate Ability Charge attributes with inputs (ChargeLMB, ChargeRMB, etc.), given that the player can rebind their abilities.
 * When we give an ability to the player we must also grant them an Ability Recharger GA associated with the input tag of the ability, using this data asset to do so.
 */
UCLASS()
class AURA_API UAbilityRechargerInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRechargerInfo> RechargerInformation;
	
	FRechargerInfo FindRechargerInfoForAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, bool bLogNotFound = false) const;
};
