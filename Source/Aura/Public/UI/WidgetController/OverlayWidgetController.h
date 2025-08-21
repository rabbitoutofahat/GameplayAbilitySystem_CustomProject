// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h" // For FGameplayTag MessageTag = FGameplayTag(); to work
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase // Creates a data table structure we can use for our data table blueprint in the UE editor; the below properties act as columns
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

/*
* Dynamic - ability to assign events in UE blueprints, Multicast - multiple blueprint widgets may want to bind to this delegate.
* 'Signature' specifies the delegate type, and is capable of broadcasting a certain data type, with a given name.
* For example, We have a signature of type FMessageWidgetRowSignature that can broadcast a value of type FUIWidgetRow which we call Row. 
* We define a member variable with the signature type, called MessageWidgetRowDelegate, which we can use to broadcast FUIWidgetRows - MessageWidgetRowDelegate.Broadcast(Row).
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable) // BlueprintType means we can use it as a type in the event graph (e.g. if we want to cast to it), Blueprintable means we can use it as a blueprint class
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:
	// Creates a widget data property for OverlayWidgetController blueprint(s) that lets us lookup tags in our MessageWidgetDataTable here in C++
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data") 
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	/*
	* Kind of an ugly line so we refactor into GetDataTableRowByTag() instead.
	* Needs a context string but we don't actually need one so pass in an empty text string.
	*/
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT("")); 
}
