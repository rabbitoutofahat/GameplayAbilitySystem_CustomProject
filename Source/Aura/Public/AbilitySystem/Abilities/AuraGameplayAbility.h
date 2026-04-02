// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	float GetManaCost(float InLevel = 1.f);
	float GetCooldown(float InLevel = 1.f);

	UPROPERTY(EditDefaultsOnly, Category = "Stacking")
	int32 MaxStacks = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Stacking")
	float RechargeDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Stacking")
	TSubclassOf<UGameplayAbility> RechargerAbility;

	void ApplyStackChangeGameplayEffect(const FGameplayAbilitySpec& Spec);
};
