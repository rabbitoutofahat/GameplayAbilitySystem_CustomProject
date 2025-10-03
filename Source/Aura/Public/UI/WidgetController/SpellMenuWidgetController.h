// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "AuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateSpellMenuSignature, bool, bSpendPointButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnStatChangedSignature OnSpellPointChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnUpdateSpellMenuSignature OnUpdateSpellMenuDelegate;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeselected();

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

private:
	FSelectedAbility SelectedAbility = { FGameplayTag().EmptyTag, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;

	// Must be static to set button states even if the Spell Menu hasn't been opened yet
	static void ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bEnableSpendPointsButton, bool& bEnableEquipButton);
};