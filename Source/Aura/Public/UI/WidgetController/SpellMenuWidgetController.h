// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateSpellMenuButtonSignature, bool, bSpendPointButtonEnabled, bool, bEquipButtonEnabled);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnStatChangedSignature OnSpellPointChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnUpdateSpellMenuButtonSignature OnUpdateSpellMenuButtonDelegate;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
};
