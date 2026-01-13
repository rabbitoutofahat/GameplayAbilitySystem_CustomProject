// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Pandemonium.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/SummonCharacter.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

void UPandemonium::EnableSpecialForAllSummons()
{
	TArray<AActor*> OutOverlappingActors;
	TArray<AActor*> ActorsToIgnore; // Will be filtering for Summon Characters within OutOverlappingActors anyway
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OutOverlappingActors, ActorsToIgnore, EffectRadius, GetAvatarActorFromActorInfo()->GetActorLocation());

	for (AActor* Actor : OutOverlappingActors)
	{
		if (ASummonCharacter* SummonedMinion = Cast<ASummonCharacter>(Actor)) SummonedMinion->ShouldEnableSpecial(true);
	}
}
