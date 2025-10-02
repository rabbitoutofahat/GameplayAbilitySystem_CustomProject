// Copyright Druid Mechanics


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnSpellPointChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints); // E.g., for Spell Globes that change from Locked to Eligible
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraPS()->OnPlayerSpellPointChangedDelegate.AddLambda(
		[this](int32 NewPoints)
		{
			OnSpellPointChangedDelegate.Broadcast(NewPoints);

			// We don't know which delegate will be broadcast first, so we make execute ShouldEnableButtons() for both, updating Ability Status and Current Spell Points as we go
			CurrentSpellPoints = NewPoints;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints);
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetAbilitySpecFromTag(AbilityTag);
	FGameplayTag StatusTag = FGameplayTag();
	if (AbilitySpec == nullptr)
	{
		StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	}
	else StatusTag = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);

	// Cache the selected ability's tags for updating spell menu button states on level up
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = StatusTag;

	ShouldEnableButtons(StatusTag, CurrentSpellPoints);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints)
{
	// Eligible status already covered by the bools' default values
	bool bEquipButtonEnabled = false;
	bool bSpendPointButtonEnabled = SpellPoints > 0;
	
	if (StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped) || StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Unlocked))
	{
		bEquipButtonEnabled = true;
	}
	if (StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Locked))
	{
		bSpendPointButtonEnabled = false;
	}

	OnUpdateSpellMenuButtonDelegate.Broadcast(bSpendPointButtonEnabled, bEquipButtonEnabled);
}
