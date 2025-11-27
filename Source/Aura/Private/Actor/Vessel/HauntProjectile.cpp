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
		// Only need to apply a gameplay effect on the server as it will change an attribute, and that attribute change will replicate to clients automatically
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Finish filling out our DamageEffectParams with the member variables that depend on a Target, i.e., the Target's ASC and the Knockback/Death-Impulse direction vectors
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			const bool bKnockback = FMath::RandRange(1.f, 100.f) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector Knockback = KnockbackDirection * DamageEffectParams.KnockbackMagnitude;
				DamageEffectParams.Knockback = Knockback;
			}

			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
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
