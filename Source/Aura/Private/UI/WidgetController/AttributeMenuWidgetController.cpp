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
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info); // Note that our widgets must bind to this delegate broadcast before we broadcast initial values
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
