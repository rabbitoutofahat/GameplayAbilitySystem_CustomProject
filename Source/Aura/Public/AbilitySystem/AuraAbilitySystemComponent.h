// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGivenSignature, UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbilitySignature, const FGameplayAbilitySpec&);

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

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	bool bStartupAbilitiesGiven = false; // Whether we bind to the AbilitiesGiven delegate before or after the broadcast happens is up to chance - we need to account for both scenarios using this boolean

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbilitySignature& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

protected:
	// AddCharacterAbilities() is only executed on the server, but the ASC's ActivatableAbilities Tag Container is replicated using OnRep_ActivateAbilities() which is virtual
	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable) // Client - this function is called on the server and will only be executed on the client that owns this ASC; Reliable - this function is guaranteed to arrive, but may be delayed
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
