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
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEquipButtonEnabled = false;
				bool bSpendPointButtonEnabled = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bSpendPointButtonEnabled, bEquipButtonEnabled);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				OnUpdateSpellMenuDelegate.Broadcast(bSpendPointButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
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

			bool bEquipButtonEnabled = false;
			bool bSpendPointButtonEnabled = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bSpendPointButtonEnabled, bEquipButtonEnabled);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			OnUpdateSpellMenuDelegate.Broadcast(bSpendPointButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag StatusTag;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(FGameplayTag().EmptyTag);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetAbilitySpecFromTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		StatusTag = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		StatusTag = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}

	// Cache the selected ability's tags for updating spell menu button states on level up
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = StatusTag;

	bool bEquipButtonEnabled = false;
	bool bSpendPointButtonEnabled = false;
	ShouldEnableButtons(StatusTag, SpellPoints, bSpendPointButtonEnabled, bEquipButtonEnabled);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	OnUpdateSpellMenuDelegate.Broadcast(bSpendPointButtonEnabled, bEquipButtonEnabled, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpellGlobeDeselected()
{
	SelectedAbility.Ability = FGameplayTag().EmptyTag;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	OnUpdateSpellMenuDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC() == nullptr) return;
	GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bEnableSpendPointsButton, bool& bEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bEnableSpendPointsButton = true;
		}
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bEnableSpendPointsButton = true;
		}
	}
}
