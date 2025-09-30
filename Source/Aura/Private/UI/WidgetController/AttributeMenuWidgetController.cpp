// Copyright Druid Mechanics


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

/*
* Take attribute initial values from our attribute set and pass them into our attribute info data set by looking up their respective tag, then broadcast to widgets bound to the attribute info delegate
*/
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (auto& Pair : GetAuraAS()->TagsToAttributes) // Key = Tag, Value = Function Pointer for Attribute Getter
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	OnAttributePointChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data) // Capture Pair by value, not reference, as it will be out of scope by the time this attribute changes and the delegate broadcasts 
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}

	GetAuraPS()->OnPlayerAttributePointChangedDelegate.AddLambda(
		[this](int32 NewPoints) 
		{
			OnAttributePointChangedDelegate.Broadcast(NewPoints);
		});

	GetAuraPS()->OnPlayerSpellPointChangedDelegate.AddLambda(
		[this](int32 NewPoints)
		{
			OnSpellPointChangedDelegate.Broadcast(NewPoints);
		});
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	// "Middle-man" function to call UpgradeAttribute on the ASC
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
    /*
	* Look up attribute tags, call the function using the function pointer stored in Pair.Value(), which returns the matching attribute (see AuraAttributeSet.h), set the attribute's
	* value from the corresponding attribute set, then broadcast the attribute's info to widgets. Note that GetNumericValue() is a static function so we need to specify which 
	* attribute set the attribute is in.
	*/ 
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
