// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Pandemonium.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/SummonCharacter.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UPandemonium::CastPandemonium()
{
	TArray<AActor*> OutOverlappingActors;
	TArray<AActor*> ActorsToIgnore; // Will be filtering for Summon Characters within OutOverlappingActors anyway
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OutOverlappingActors, ActorsToIgnore, EffectRadius, GetAvatarActorFromActorInfo()->GetActorLocation());

	for (AActor* Actor : OutOverlappingActors)
	{
		if (ASummonCharacter* SummonedMinion = Cast<ASummonCharacter>(Actor))
		{		
			FGameplayTagContainer TagContainer = FGameplayTagContainer();
			TagContainer.AddTag(FAuraGameplayTags::Get().Buff_DemonicStrength);
			SummonedMinion->GetAbilitySystemComponent()->AddLooseGameplayTags(TagContainer);
			SummonedMinion->ShouldEnableSpecial(true);
		}
	}
}
