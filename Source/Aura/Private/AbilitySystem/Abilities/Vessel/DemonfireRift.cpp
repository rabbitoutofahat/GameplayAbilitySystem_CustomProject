// Roland Frances


#include "AbilitySystem/Abilities/Vessel/DemonfireRift.h"
#include "Actor/ProjectileSpawner.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Vessel/SummonDregling.h"
#include "AuraGameplayTags.h"
#include "Character/PlayableClasses/Vessel.h"

void UDemonfireRift::SpawnRift()
{
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Left = Forward.RotateAngleAxis(-90.f, FVector::UpVector);
	const float RandomSpread = FMath::RandRange(0.f, RiftSpawnSpread);

	const FVector SpawnDirection = Left.RotateAngleAxis(RandomSpread, FVector::UpVector);
	FVector SpawnLocation = Location + SpawnDirection * FMath::FRandRange(RiftSpawnDistanceMin, RiftSpawnDistanceMax);

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	AProjectileSpawner* Rift = GetWorld()->SpawnActorDeferred<AProjectileSpawner>(
		RiftClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Rift->Owner = GetAvatarActorFromActorInfo();
	Rift->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	AVessel* Vessel = Cast<AVessel>(GetAvatarActorFromActorInfo());
	UAuraAbilitySystemComponent* VesselASC = Cast<UAuraAbilitySystemComponent>(Vessel->GetAbilitySystemComponent());
	
	if (VesselASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_DemonfireRift_RiftMastery)) Rift->NumProjectiles += AdditionalBolts;
	if (VesselASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SummonDregling_PortentOfDestruction))
	{
		/*
		* "Open rift" -> determine whether to grant the buff associated with this upgrade passive
		* Use the static getter on USummonDregling Ability Class to get the proc chance for this passive
		*/
		const float ProcChance = USummonDregling::GetPortentProcChance(VesselASC);
		if (FMath::RandRange(0.f, 1.f) <= ProcChance) VesselASC->ApplyGameplayEffectToSelf(Vessel->PortentOfDestructionBuffClass.GetDefaultObject(), 1.f, VesselASC->MakeEffectContext());
	}
	
	Rift->SetActorEnableCollision(false);
	Rift->FinishSpawning(SpawnTransform);
}
