// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// If we're going to trace our way up to the HUD, we need to do so using an object that exists, but a static function cannot access any objects that exist in the world,
	// because the class itself that the static function belongs to may not exist in the world. This is why many library functions need a world context object, i.e., a reference 
	// it can use to trace to something in the world that we want to affect.
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
};
