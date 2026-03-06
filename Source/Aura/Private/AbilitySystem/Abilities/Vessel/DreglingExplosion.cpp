// Roland Frances


#include "AbilitySystem/Abilities/Vessel/DreglingExplosion.h"
#include "Character/Summons/Dregling.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemComponent.h"

void UDreglingExplosion::StartDissolve()
{
	ASummonCharacter* Dregling = Cast<ASummonCharacter>(GetAvatarActorFromActorInfo());
	Dregling->SetActorEnableCollision(false); // Don't want the dregling to die early (later summoned demons won't be able to be targeted or get hit anyway)
	Dregling->Dissolve();
}

void UDreglingExplosion::Explosion()
{
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	ADregling* Dregling = Cast<ADregling>(GetAvatarActorFromActorInfo());

	AVessel* Vessel = Cast<AVessel>(Dregling->OwnerActor);
	UAbilitySystemComponent* VesselASC = Vessel->GetAbilitySystemComponent();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Vessel);
	TArray<AActor*> ActorsToDamage;
	DamageEffectParams.RadialDamageOrigin = Dregling->GetActorLocation();

	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(DamageEffectParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, DamageEffectParams.RadialDamageOuterRadius, DamageEffectParams.RadialDamageOrigin);

	for (AActor* Actor : ActorsToDamage)
	{
		if (!Actor->ActorHasTag(FName("Enemy"))) continue;
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
		{
			if (VesselASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling_MoltenTouch))
			{
				Cast<AAuraCharacterBase>(Actor)->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Dregling->MoltenTouchDebuffClass.GetDefaultObject(), 1.f, VesselASC->MakeEffectContext());
			}

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