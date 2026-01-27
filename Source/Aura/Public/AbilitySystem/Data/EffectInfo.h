// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EffectInfo.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAuraEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;
};

/**
 * Used to store and grant Passive Gameplay Effects to Actors that purchase upgrades for their abilities 
 */
UCLASS()
class AURA_API UEffectInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EffectInformation")
	TArray<FAuraEffectInfo> EffectInformation;

	FAuraEffectInfo FindEffectInfoForTag(const FGameplayTag& EffectTag, bool bLogNotFound = false) const;
};
