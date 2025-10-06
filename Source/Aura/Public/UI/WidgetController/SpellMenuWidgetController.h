// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateSpellMenuSignature, bool, bSpendPointButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

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

	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature OnSpellPointChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateSpellMenuSignature OnUpdateSpellMenuDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate; // Play an animation for either the Offensive or Passive Spell section of the EquippedSpellRow when pressing the Equip button

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate; // Cancel out of "Wait for Equip" widget animation/state when deselecting a Spell Globe or selecting a different Spell Globe

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeselected();

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& InputSlot, const FGameplayTag& AbilityType);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& NewInputSlot, const FGameplayTag& OldInputSlot);

private:
	FSelectedAbility SelectedAbility = { FGameplayTag().EmptyTag, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	FGameplayTag SelectedSlot;

	// Must be static to set button states even if the Spell Menu hasn't been opened yet
	static void ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bEnableSpendPointsButton, bool& bEnableEquipButton);

	bool bWaitingForEquipSelection = false; // Ensures we only play the EquippedSpellRow's "Hide Selection" animations if we are currently waiting to equip

};