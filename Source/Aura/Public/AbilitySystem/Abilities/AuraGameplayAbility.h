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
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Ability Charges")
	int32 GetCurrentAbilityCharges() { return CurrentCharges; }

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

protected:
	float GetManaCost(float InLevel = 1.f);
	float GetCooldown(float InLevel = 1.f);

	UPROPERTY(EditDefaultsOnly, Category = "Ability Charges")
	int32 MaxCharges = 1;

	UPROPERTY()
	int32 CurrentCharges = MaxCharges;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Charges")
	float RechargeDuration = -1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Charges")
	TSubclassOf<UGameplayAbility> RechargerAbility;

	/*
	* After an ability has been granted through the ASC, the charge attributes are automatically changed to the reflect the ability's Max Charges.
	* This is possible using a dynamic GE created at runtime.
	*/
	void InitialiseChargeCountAttributes(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
};
