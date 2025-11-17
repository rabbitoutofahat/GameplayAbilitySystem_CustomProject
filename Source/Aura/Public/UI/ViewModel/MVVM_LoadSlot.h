// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

// The widget switcher has access to the Load Slot view model at the moment it is assigned to its children widgets, where we can broadcast this delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	void InitialiseSlot(); // Called whenever we need to change the slot status, have this class broadcast its slot index based on whatever its status is whenever we load in data

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString MapAssetName;

private:
	/* -- Field Notifies --
	* 
	* Create a Field Notify variable that corresponds to a widget element we wish to change, then bind to that element in the relevant widget blueprint.
	* (These member variables are preferred to be private on the course, but field notifies require blueprint read-write access which contradicts this, so we use meta specifiers.)
	* 
	* For example, we create a PlayerName Field Notify in this Load Slot View Model, which can then be bound to the Text_PlayerName in BP_LoadSlot_Taken.
	* Now if we make changes to PlayerName in C++, those changes should be reflected in Text_PlayerName in-engine.
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"));
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"));
	int32 PlayerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"));
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString SlotName; // As of 5.3, every Load Slot blueprint needs a Field Notify bound to it before we can successfully set its Load Slot View Model in blueprint, so we use SlotName

public:
	/* Field Notify Setters & Getters */

	void SetPlayerName(FString InPlayerName);
	void SetPlayerLevel(int32 InPlayerLevel);
	void SetMapName(FString InMapName);
	void SetSlotName(FString InSlotName);

	FString GetPlayerName() const { return PlayerName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	FString GetMapName() const { return MapName; }
	FString GetSlotName() const { return SlotName; }
};