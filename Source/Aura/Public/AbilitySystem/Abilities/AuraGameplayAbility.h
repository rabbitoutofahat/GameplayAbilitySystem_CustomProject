// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

class UAbilityRechargerInfo;

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Ability Charges")
	int32 GetCurrentAbilityCharges() { return CurrentCharges; }

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	float GetManaCost(float InLevel = 1.f);
	float GetCooldown(float InLevel = 1.f);

	UPROPERTY(EditDefaultsOnly, Category = "Ability Charges")
	int32 MaxCharges = 1;

	UPROPERTY()
	int32 CurrentCharges = MaxCharges;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Charges")
	float ChargeCooldown = -1.f; // "Cooldown" between use of ability charges

	UPROPERTY(EditDefaultsOnly, Category = "Ability Charges")
	TObjectPtr<UAbilityRechargerInfo> AbilityRechargerInfo;

	/*
	* After an ability has been granted through the ASC, the charge attributes are automatically changed to the reflect the ability's Max Charges.
	* This is possible using a dynamic GE created at runtime.
	*/
	void InitialiseChargeCountAttributes(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

private:
	float AbilityCooldown = 0.f; // Cache the base cooldown of the ability for use when modifying the cooldown based on remaining charges
};
