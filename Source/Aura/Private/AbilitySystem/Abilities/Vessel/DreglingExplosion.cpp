// Roland Frances


#include "AbilitySystem/Abilities/Vessel/DreglingExplosion.h"
#include "Character/SummonCharacter.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"

void UDreglingExplosion::StartDissolve()
{
	ASummonCharacter* Dregling = Cast<ASummonCharacter>(GetAvatarActorFromActorInfo());
	Dregling->SetActorEnableCollision(false); // Don't want the dregling to die early (later summoned demons won't be able to be targeted and hit anyway)
	Dregling->Dissolve();
}

void UDreglingExplosion::Explosion()
{
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	ASummonCharacter* Dregling = Cast<ASummonCharacter>(GetAvatarActorFromActorInfo());

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Cast<AVessel>(DamageEffectParams.WorldContextObject));
	TArray<AActor*> ActorsToDamage;
	DamageEffectParams.RadialDamageOrigin = Dregling->GetActorLocation();

	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(DamageEffectParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, DamageEffectParams.RadialDamageOuterRadius, DamageEffectParams.RadialDamageOrigin);

	for (AActor* Actor : ActorsToDamage)
	{
		if (!Actor->ActorHasTag(FName("Enemy"))) continue;
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
		{
			const FVector DeathImpulse = Dregling->GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
		}
	}

	FGameplayCueParameters CueParams;
	CueParams.Location = Dregling->GetActorLocation();
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(Dregling->GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	Dregling->Destroy();
}