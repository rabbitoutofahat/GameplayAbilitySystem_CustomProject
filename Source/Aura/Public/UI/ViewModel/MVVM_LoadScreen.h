// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitialiseLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot); // Vacant slot

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName); // EnterName slot

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot); // Taken slot (Slot details such as player name and level will be populated from the view model)

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	void LoadData();
	
private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0; // 0 = Vacant slot

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1; // 1 = EnterName slot
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2; // 2 = Taken slot

	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;
};
