// Copyright Druid Mechanics


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet); // Cast (and check) attribute set so we can use the attribute set's attribute accessors
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) // [Class capture] (Input parameter list) {Function body}
		/*
		* When the Aura ASC broadcasts to this delegate, the lambda (aka an anonymous function) will be called, receiving the broadcast from the delegate as the input, given the input
		* parameter type is a match (an asset tag container is being broadcast in this case so the input needs to be of type FGameplayTagContainer), and execute the contents of the
		* function body. Adding a lambda means we don't have to declare callback functions for all of the delegates we want to bind to, reflected in the attribute change delegates below.
		* We want to use GetDataTableRowByTag, which is a member function of the class OverlayWidgetController, so we must capture that class in the lambda in the square brackets (hence [this]).
		*/
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				/*
				* Checks to see if a given asset tag is a message tag (I.e., "Message" is the root of the tag hierarchy).
				* For example, say Tag = Message.HealthPotion :
				* "Message.HealthPotion".MatchesTag("Message") will return True,
				* "Message".MatchesTag("Message.HealthPotion") will return False.
				*/
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag); // Returns a pointer to the row in the message widget data table associated with the given tag
					MessageWidgetRowDelegate.Broadcast(*Row); // Broadcasting to the message widget
				}
			}
		}
	);

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
}

