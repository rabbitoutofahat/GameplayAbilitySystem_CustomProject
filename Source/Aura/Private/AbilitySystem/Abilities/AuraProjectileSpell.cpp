// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Public/AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectileAtSocket(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	// Want to spawn the projectile on the server
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag); // GetCombatSocketLocation is a Blueprint Native Event so we call Execute_GetCombatSocketLocation()
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion()); // Convert FRotator to FQuat
	SpawnProjectile(SpawnTransform);
}

void UAuraProjectileSpell::SpawnProjectilesAboveActor(const FVector& ProjectileTargetLocation, const int32 NumProjectiles, const float SpawnDistance)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FVector> Locations = UAuraAbilitySystemLibrary::EvenlyRotatedVectors(FVector::UpVector * SpawnDistance, Forward, ProjectileSpread, NumProjectiles);

	for (const FVector& Location : Locations)
	{
		FTransform SpawnTransform;
		const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Location;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation((ProjectileTargetLocation - SpawnLocation).ToOrientationQuat());
		SpawnProjectile(SpawnTransform);
	}
}

void UAuraProjectileSpell::SpawnProjectile(FTransform& SpawnTransform)
{
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
}