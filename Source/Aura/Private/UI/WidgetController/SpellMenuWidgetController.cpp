// Copyright Druid Mechanics


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"

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
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	// Eligible status already covered by the bools' default values
	bool bEquipButtonEnabled = false;
	bool bSpendPointButtonEnabled = GetAuraPS()->GetSpellPoints() > 0;

	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetAbilitySpecFromTag(AbilityTag);
	if (AbilitySpec == nullptr)
	{
		// Once the Spell Menu is filled out, this if check should simply return

		bSpendPointButtonEnabled = false; 
		OnUpdateSpellMenuButtonDelegate.Broadcast(bSpendPointButtonEnabled, bEquipButtonEnabled);
		return;
	}

	FGameplayTag StatusTag = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
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