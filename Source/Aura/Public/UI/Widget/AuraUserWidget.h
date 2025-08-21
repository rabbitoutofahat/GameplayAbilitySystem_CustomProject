// Copyright Druid Mechanics 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * Dependencies will be one-way only, from UserWidget to WidgetController
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); // Ties together WidgetController and WidgetControllerSet

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; // Responsible for what widgets will look like on-screen when they receive data, i.e. should produce a visual response

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet(); // When we set the widget controller for a given widget, call this function to implement everything in the blueprint that we want to do in response to the widget controller set

};
