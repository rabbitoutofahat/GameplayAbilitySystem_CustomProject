// Roland Frances


#include "Character/PlayableClasses/Vessel.h"
#include "GameplayAbilitySpec.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"

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
		/*
		* "Open rift" -> determine whether to grant the buff associated with this upgrade passive
		* Use the static getter on USummonDregling Ability Class to get the proc chance for this passive 
		*/
		const float ProcChance = USummonDregling::GetPortentProcChance(Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()));
		if (FMath::RandRange(0.f, 1.f) <= ProcChance) GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(PortentOfDestructionBuffClass.GetDefaultObject(), 1.f, GetAbilitySystemComponent()->MakeEffectContext());
	}

	return Summon;
}