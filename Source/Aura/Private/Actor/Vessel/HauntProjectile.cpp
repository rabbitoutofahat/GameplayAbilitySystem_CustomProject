// Roland Frances


#include "Actor/Vessel/HauntProjectile.h"
#include "Character/SummonCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/PlayableClasses/Vessel.h"

void AHauntProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!bHit) PlayImpactEffects();

	if (HasAuthority())
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Cast<AVessel>(DamageEffectParams.WorldContextObject)); // TODO: If multiplayer, add all player characters
		ActorsToIgnore.Add(Cast<AVessel>(DamageEffectParams.WorldContextObject)->DemonicSoul);
		TArray<AActor*> ActorsToDamage;
		DamageEffectParams.RadialDamageOrigin = GetActorLocation();
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(DamageEffectParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, DamageEffectParams.RadialDamageOuterRadius, DamageEffectParams.RadialDamageOrigin);

		for (AActor* Actor : ActorsToDamage)
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
			{
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
			}
		}

		Destroy();
	}
	else bHit = true;
}

void AHauntProjectile::PlayImpactEffects()
{
	Super::PlayImpactEffects();
	if (ASummonCharacter* DemonicSoul = Cast<AVessel>(DamageEffectParams.WorldContextObject)->DemonicSoul)
	{
		DemonicSoul->SetActorLocation(GetActorLocation());
		UAuraAbilitySystemLibrary::HideSummon(DemonicSoul, false);
	}
}
