// Copyright Druid Mechanics


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitialiseLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;

	LoadSlots.Add(0, LoadSlot_0); // 0 = Vacant slot
	LoadSlots.Add(1, LoadSlot_1); // 1 = EnterName slot
	LoadSlots.Add(2, LoadSlot_2); // 2 = Taken slot

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[Slot]->SlotStatus = Taken;

	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitialiseSlot();
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false); // Want to disable the button once we've clicked it
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true); // Want to enable the button on all the other load slots
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitialiseSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true); // As we're choosing whether to delete the slot, we can enable the Select Slot button for the next time we return to Taken state for that particular Slot
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (IsValid(SelectedSlot)) AuraGameMode->TravelToMap(SelectedSlot);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetSlotName(), LoadSlot.Key);
		TEnumAsByte<ESaveSlotStatus> SlotStatus = SaveObject->SlotStatus;

		LoadSlot.Value->SlotStatus = SlotStatus;
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->InitialiseSlot();
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}