// Roland Frances


#include "Character/PlayableClasses/Vessel.h"
#include "GameplayAbilitySpec.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

ASummonCharacter* AVessel::SpawnSummonedMinion(UClass* Class, const FTransform& SpawnTransform, int32 OwnerLevel)
{
	ASummonCharacter* Summon = GetWorld()->SpawnActorDeferred<ASummonCharacter>(Class, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Summon->OwnerActor = this;
	Summon->SetLevel(OwnerLevel); // Set after SpawnSummonedMinion is called in blueprint

	for (const TSubclassOf<UGameplayAbility> AbilityClass : Summon->GetStartupAbilities())
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, OwnerLevel);
		Summon->GetAbilitySystemComponent()->GiveAbility(AbilitySpec); // Give the summon its Special Ability
	}

	Summon->FinishSpawning(SpawnTransform);
	Summon->SpawnDefaultController();

	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_DemonicSoul_AbyssalDominion))
	{
		Summon->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(AbyssalDominionBuffClass.GetDefaultObject(), 1.f, Summon->GetAbilitySystemComponent()->MakeEffectContext());
	}

	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling_PortentOfDestruction))
	{
		GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(PortentOfDestructionBuffClass.GetDefaultObject(), 1.f, Summon->GetAbilitySystemComponent()->MakeEffectContext());
	}

	return Summon;
}