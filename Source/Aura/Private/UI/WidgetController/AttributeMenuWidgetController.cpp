// Copyright Druid Mechanics


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

/*
* Take attribute initial values from our attribute set and pass them into our attribute info data set by looking up their respective tag, then broadcast to widgets bound to the attribute info delegate
*/
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes) // Key = Tag, Value = Function Pointer for Attribute Getter
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data) // Capture Pair by value, not reference, as it will be out of scope by the time this attribute changes and the delegate broadcasts 
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
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
