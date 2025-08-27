// Copyright Druid Mechanics


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

/*
* Take attribute initial values from our attribute set and pass them into our attribute info data set by looking up their respective tag, then broadcast to widgets bound to the attribute info delegate
*/
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes) // Key = Tag, Value = Attribute Signature Delegate
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		FGameplayAttribute Attr = Pair.Value(); // Call the function using the function pointer, which returns the matching attribute (see AuraAttributeSet.h)
		Info.AttributeValue = Attr.GetNumericValue(AS); // GetNumericValue() is a static function so we need to specify which attribute set the attribute is in
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
