// Roland Frances


#include "Actor/Vessel/SoulOrb.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

void ASoulOrb::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVessel* Vessel = Cast<AVessel>(OtherActor);
	if (Vessel == nullptr) return;

	UAbilitySystemComponent* VesselASC = Vessel->GetAbilitySystemComponent();
	VesselASC->ApplyGameplayEffectToSelf(SoulOrbPassiveEffectClass.GetDefaultObject(), 1.f, VesselASC->MakeEffectContext());
	PlayImpactEffects();

	if (VesselASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon_VengefulManifestation))
	{
		float RNG = FMath::FRandRange(0.f, 100.f);
		if (RNG <= VengefulSpiritSpawnPercent)
		{
			const FVector Location = Vessel->GetActorLocation();
			const FVector Forward = Vessel->GetActorForwardVector();
			const FVector Left = Forward.RotateAngleAxis(-90.f, FVector::UpVector);
			const float RandomSpread = FMath::RandRange(0.f, SpiritSpawnSpread);

			const FVector SpawnDirection = Left.RotateAngleAxis(RandomSpread, FVector::UpVector);
			FVector SpawnLocation = Location + SpawnDirection * FMath::FRandRange(SpiritSpawnDistanceMin, SpiritSpawnDistanceMax);

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLocation);
			ASummonCharacter* VengefulSpirit = Vessel->SpawnSummonedMinion(VengefulSpiritClass, SpawnTransform, Vessel->GetPlayerLevel_Implementation());
			// TODO: Should implement line trace in case the player is on an uneven surface, depends on map design direction
		}
	}

	Destroy();
}
