// Roland Frances


#include "AbilitySystem/Abilities/Vessel/SoulSiphon.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "NiagaraFunctionLibrary.h"

void USoulSiphon::ApplyDamageToTarget(AActor* ActorToDamage)
{
	if (!IsValid(ActorToDamage)) return;
	CauseDamage(ActorToDamage);
}

// Spawn soul orbs with different initial trajectories that will travel from the target to the ability owner (travel path handled in blueprint)
TArray<UNiagaraComponent*> USoulSiphon::SpawnSoulOrbsAtTarget(AActor* DamagedActor)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return TArray<UNiagaraComponent*>();

	TArray<UNiagaraComponent*> SoulOrbs;
	FRotator Rotation = (GetAvatarActorFromActorInfo()->GetActorLocation() - DamagedActor->GetActorLocation()).Rotation(); // Want to spawn orbs facing towards the ability owner
	TArray<FRotator> SpawnRotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Rotation.Vector(), FVector::UpVector, OrbSpread, 3); // Always spawn 3 orbs so hard coding it here

	for (const FRotator& SpawnRotation : SpawnRotations)
	{
		FVector SpawnLocation = SpawnRotation.Vector() + DamagedActor->GetActorForwardVector() * OrbSpawnDistance + DamagedActor->GetActorLocation();
		UNiagaraComponent* SoulOrb = UNiagaraFunctionLibrary::SpawnSystemAtLocation(DamagedActor, SoulOrbEffect, SpawnLocation, SpawnRotation);
		SoulOrbs.Add(SoulOrb);
	}

	return SoulOrbs;
}
