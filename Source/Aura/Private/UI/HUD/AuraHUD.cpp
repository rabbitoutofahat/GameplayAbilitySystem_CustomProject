// Copyright Druid Mechanics


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

template<typename T>
T* AAuraHUD::GetWidgetController(T* WidgetController, TSubclassOf<T> WidgetControllerClass, const FWidgetControllerParams& WCParams)
{
	if (WidgetController == nullptr) // Will construct the overlay widget controller the first time the function is called, and return it all subsequent times
	{
		WidgetController = NewObject<T>(this, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(WCParams); // Key attributes like health, mana, etc, are set
	    WidgetController->BindCallbacksToDependencies(); // When attributes or other dependencies are changed, those changes are broadcast
	}
	return WidgetController;
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	return GetWidgetController<UOverlayWidgetController>(OverlayWidgetController, OverlayWidgetControllerClass, WCParams);;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	return GetWidgetController<UAttributeMenuWidgetController>(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass, WCParams);
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialised, please fill out BP_AuraHUD")); // Check condition then print formatted string to the crash log if the check fails
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialised, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass); // Creating our own local UUserWidget class called Widget
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetWidgetController<UOverlayWidgetController>(OverlayWidgetController, OverlayWidgetControllerClass, WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues(); // As long as we're setting the widget controller and binding the blueprint events to it, then we can call BroadcastInitialValues
	Widget->AddToViewport();
}

