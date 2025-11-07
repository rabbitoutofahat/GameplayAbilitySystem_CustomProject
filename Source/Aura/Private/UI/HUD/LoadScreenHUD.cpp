// Copyright Druid Mechanics


#include "UI/HUD/LoadScreenHUD.h"
#include "UI/Widget/LoadScreenWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	// Construct View Models for the Load Screen and its 3 Load Slots 
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitialiseLoadSlots();

	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitialiseWidget(); // Assign View Models to the Load Slot Widgets

	LoadScreenViewModel->LoadData(); // On startup, should show the correct Load Slot Widget based on the status (Vacant, EnterName, Taken) that we've saved
}
