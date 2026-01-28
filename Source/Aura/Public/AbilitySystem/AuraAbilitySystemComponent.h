// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature);
DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedSignature, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FiveParams(FAbilityEquippedSignature, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const int32 /*Level*/, const FGameplayTag& /*NewInputSlot*/, const FGameplayTag& /*OldInputSlot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveSignature, const FGameplayTag& /*AbilityTag*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveSignature, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FEffectStatusChangedSignature, const FGameplayTag& /*EffectTag*/, const FGameplayTag& /*StatusTag*/);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet(); // Allows Aura ASC to bind to delegates

	FEffectAssetTagsSignature EffectAssetTagsDelegate;
	FAbilitiesGivenSignature AbilitiesGivenDelegate;
	FAbilityStatusChangedSignature AbilityStatusChangedDelegate;
	FAbilityEquippedSignature AbilityEquippedDelegate;
	FDeactivatePassiveSignature DeactivatePassiveDelegate;
	FActivatePassiveSignature ActivatePassiveDelegate;
	FEffectStatusChangedSignature EffectStatusChangedDelegate;

	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	bool bStartupAbilitiesGiven = false; // Whether we bind to the AbilitiesGiven delegate before or after the broadcast happens is up to chance - we need to account for both scenarios using this boolean

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbilitySignature& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromEffectSpec(const FGameplayEffectSpec& EffectSpec);

	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag); // Can't be static because it relies on GetStatusTagFromSpec()
	FGameplayTag GetInputFromAbilityTag(const FGameplayTag& AbilityTag); 

	bool InputSlotIsEmpty(const FGameplayTag& Slot);
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	UFUNCTION(NetMulticast, Unreliable) // Unreliable - this function may be dropped if network congestion occurs
	void MulticastActivatePassiveAbility(const FGameplayTag& AbilityTag, bool bActivate);
	 
	FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetAbilitySpecFromSlot(const FGameplayTag& Slot);
	FGameplayEffectSpec* GetEffectSpecFromTag(const FGameplayTag& EffectTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpdateAbilityStatuses(int32 Level);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	void MakeAbilityUpgradesEligible(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputSlot);

	UFUNCTION(Client, Reliable) // Allows clients to equip abilities on the spell menu
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const int32 Level, const FGameplayTag& NewInputSlot, const FGameplayTag& OldInputSlot);

	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	static void ClearSlot(FGameplayAbilitySpec* Spec); // Clear the ability of its input slot
	void ClearAbilitiesOfSlot(const FGameplayTag& InputSlot); // Clear the input slot of any equipped abilities

protected:
	// AddCharacterAbilities() is only executed on the server, but the ASC's ActivatableAbilities Tag Container is replicated using OnRep_ActivateAbilities() which is virtual
	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable) // Client - this function is called on the server and will only be executed on the client that owns this ASC; Reliable - this function is guaranteed to arrive, but may be delayed
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel);

	UFUNCTION(Client, Reliable)
	void ClientUpdateEffectStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
};
