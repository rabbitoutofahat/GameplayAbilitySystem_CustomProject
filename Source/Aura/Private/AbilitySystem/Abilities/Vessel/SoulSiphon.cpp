// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SoulSiphon.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/EnemyInterface.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AuraGameplayTags.h"
#include "Character/PlayableClasses/Vessel.h"
#include "GameplayCueManager.h"

void USoulSiphon::SoulSiphon(AActor* TargetActor)
{
	if (!IsValid(TargetActor) || !TargetActor->Implements<UEnemyInterface>()) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (ASC && ASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon_SoulCrush)) // Upgrade changes Soul Siphon from single target to radial damage
	{
		FDamageEffectParams SoulCrushParams = MakeDamageEffectParamsFromClassDefaults();
		SoulCrushParams.bIsRadialDamage = true;
		SoulCrushParams.RadialDamageInnerRadius = SoulCrushInnerRadius;
		SoulCrushParams.RadialDamageOuterRadius = SoulCrushOuterRadius;
		SoulCrushParams.RadialDamageOrigin = TargetActor->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Cast<AVessel>(SoulCrushParams.WorldContextObject)); 
		TArray<AActor*> ActorsToDamage;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(SoulCrushParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, SoulCrushParams.RadialDamageOuterRadius, SoulCrushParams.RadialDamageOrigin);

		for (AActor* Actor : ActorsToDamage)
		{
			if (!Actor->ActorHasTag(FName("Enemy"))) continue;
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
			{
				const FVector DeathImpulse = (Actor->GetActorLocation() - SoulCrushParams.RadialDamageOrigin) * SoulCrushParams.DeathImpulseMagnitude;
				SoulCrushParams.DeathImpulse = DeathImpulse;
				SoulCrushParams.TargetAbilitySystemComponent = TargetASC;
				UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(SoulCrushParams);
				if (Actor == TargetActor) SpawnSoulOrbsAtTarget(Actor, 3); // Spawn 3 orbs on primary target
				else SpawnSoulOrbsAtTarget(Actor, 1); // Spawn 1 orb on each target hit by the radial damage
			}
		}

		FGameplayCueParameters CueParams;
		CueParams.Location = TargetActor->GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetAvatarActorFromActorInfo(), FAuraGameplayTags::Get().GameplayCue_SoulCrush, CueParams);
	}
	else 
	{
		CauseDamage(TargetActor);
		SpawnSoulOrbsAtTarget(TargetActor, 3); 
	}
}

void USoulSiphon::SpawnSoulOrbsAtTarget(AActor* DamagedActor, int32 NumOrbs)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FRotator Rotation = (GetAvatarActorFromActorInfo()->GetActorLocation() - DamagedActor->GetActorLocation()).Rotation(); // Want to spawn orbs facing towards the ability owner
	TArray<FRotator> SpawnRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Rotation.Vector(), FVector::UpVector, OrbSpread, NumOrbs); // Always spawn 3 orbs on primary target, 1 otherwise

	for (const FRotator& SpawnRotation : SpawnRotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(DamagedActor->GetActorLocation() + (Rotation.Vector() * OrbSpawnDistance));
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());

		AAuraProjectile* SoulOrb = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			SoulOrbClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		AActor* HomingTarget = GetAvatarActorFromActorInfo();
		SoulOrb->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		SoulOrb->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		SoulOrb->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		SoulOrb->FinishSpawning(SpawnTransform);
	}
}
