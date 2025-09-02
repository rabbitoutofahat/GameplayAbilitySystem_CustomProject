// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	// We create an ability task to send mouse cursor data from client to server as the mouse cursor hit location is not known on the server and not automatically predicted
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouse* GetTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData; // Becomes an output execution pin in the GetTargetDataUnderMouse() blueprint node

private:
	virtual void Activate() override; // Automatically called when an ability task is started

	void SendMouseCursorData(); // Call this function if we are locally controlled
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag); // Call this function if we are not locally controlled, i.e., the data handle needs to be replicated from the owning client to the server
};
