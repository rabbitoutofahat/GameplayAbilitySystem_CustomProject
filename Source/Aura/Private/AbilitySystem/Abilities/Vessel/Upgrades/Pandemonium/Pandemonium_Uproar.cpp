// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Upgrades/Pandemonium/Pandemonium_Uproar.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Character/AuraCharacter.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/Vessel/Pandemonium.h"

FString UPandemonium_Uproar::GetDescription()
{
	return FString::Printf(TEXT(
		// Title
		"<Title>Uproar</>\n"

		// Description
		"<Default>Increases the number of ability charges by %d</>"),
		NumCharges);
}

void UPandemonium_Uproar::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	AAuraCharacter* Character = Cast<AAuraCharacter>(ActorInfo->AvatarActor);
	if (!Character) return;

	UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(Character->GetAbilitySystemComponent());
	const FGameplayTag PandemoniumTag = FAuraGameplayTags::Get().Abilities_Vessel_Pandemonium;
	UPandemonium* PandemoniumAbility = Cast<UPandemonium>(ASC->GetAbilitySpecFromTag(PandemoniumTag)->Ability);
	
	NumCharges = PandemoniumAbility->NumAdditionalCharges;
	UAuraAbilitySystemLibrary::IncreaseAbilityCharges(NumCharges, PandemoniumTag, ASC);
	PandemoniumAbility->InitialiseChargeCountAttributes(ActorInfo, Spec); // Reinitialise the charge count attributes to update the ASC with the new max charges value
}
