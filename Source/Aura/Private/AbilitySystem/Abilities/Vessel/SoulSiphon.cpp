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

void USoulSiphon::ApplyDamageToTarget(AActor* ActorToDamage)
{
	if (!IsValid(ActorToDamage) || !ActorToDamage->Implements<UEnemyInterface>()) return;
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (ASC && ASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon_SoulCrush)) // Changes from single target to radial damage
	{
		FDamageEffectParams SoulCrushParams = MakeDamageEffectParamsFromClassDefaults();
		SoulCrushParams.bIsRadialDamage = true;
		SoulCrushParams.RadialDamageInnerRadius = SoulCrushInnerRadius;
		SoulCrushParams.RadialDamageOuterRadius = SoulCrushOuterRadius;
		SoulCrushParams.RadialDamageOrigin = ActorToDamage->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Cast<AVessel>(SoulCrushParams.WorldContextObject)); 
		TArray<AActor*> ActorsToDamage;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(SoulCrushParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, SoulCrushParams.RadialDamageOuterRadius, SoulCrushParams.RadialDamageOrigin);

		for (AActor* Actor : ActorsToDamage)
		{
			if (!Actor->ActorHasTag(FName("Enemy"))) continue; // With the addition of more tags beyond "Player" and "Enemy", we need to if statement to filter out all non-enemies
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
			{
				const FVector DeathImpulse = (Actor->GetActorLocation() - SoulCrushParams.RadialDamageOrigin) * SoulCrushParams.DeathImpulseMagnitude;
				SoulCrushParams.DeathImpulse = DeathImpulse;
				SoulCrushParams.TargetAbilitySystemComponent = TargetASC;
				UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(SoulCrushParams);
				SpawnSoulOrbsAtTarget(Actor, 1); // Spawn 1 orb on each target hit by the radial damage
			}
		}
	}
	else 
	{
		CauseDamage(ActorToDamage);
		SpawnSoulOrbsAtTarget(ActorToDamage, 3); // Spawn 3 orbs on primary target
	}
}

// Spawn soul orbs with different initial trajectories that will travel from the target to the ability owner (travel path handled in blueprint)
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
