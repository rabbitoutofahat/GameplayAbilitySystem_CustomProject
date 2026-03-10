// Roland Frances


#include "Actor/Vessel/DreglingProjectile.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BrainComponent.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Character/Summons/Dregling.h"

void ADreglingProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) PlayImpactEffects();

	if (HasAuthority())
	{
		AVessel* Vessel = Cast<AVessel>(DamageEffectParams.WorldContextObject);
		UAbilitySystemComponent* VesselASC = Vessel->GetAbilitySystemComponent();
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Vessel); // TODO: If multiplayer, add all player characters
		TArray<AActor*> ActorsToDamage;
		DamageEffectParams.RadialDamageOrigin = GetActorLocation();
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(DamageEffectParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, DamageEffectParams.RadialDamageOuterRadius, DamageEffectParams.RadialDamageOrigin);

		for (AActor* Actor : ActorsToDamage)
		{
			if (!Actor->ActorHasTag(FName("Enemy"))) continue; // With the addition of more tags beyond "Player" and "Enemy", we need to if statement to filter out all non-enemies
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
			{
				if (VesselASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling_MoltenTouch) && bIsPrimaryProjectile)
				{
					ADregling* Dregling = Cast<ADregling>(ClassToSpawn.GetDefaultObject());
					Cast<AAuraCharacterBase>(Actor)->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Dregling->MoltenTouchDebuffClass.GetDefaultObject(), 1.f, VesselASC->MakeEffectContext());
				}

				const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulse;
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
			}
		}

		Destroy();
	}
	else bHit = true;
}

void ADreglingProjectile::PlayImpactEffects()
{
	Super::PlayImpactEffects();

	if (AVessel* Vessel = Cast<AVessel>(DamageEffectParams.WorldContextObject))
	{
		ASummonCharacter* Dregling = Vessel->SpawnSummonedMinion(ClassToSpawn, FTransform(GetActorRotation(), GetActorLocation()), Vessel->GetPlayerLevel_Implementation());
		Cast<AAuraAIController>(Dregling->GetController())->GetBrainComponent()->StopLogic("Start Spawning Actor");
		Dregling->bIsBeingSpawned = true;
	}
}