// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Haunt.h"
#include "Actor/Vessel/HauntProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameplayCueManager.h"
#include "AuraGameplayTags.h"

void UHaunt::SpawnReturnProjectile()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ASummonCharacter* DemonicSoul = Cast<AVessel>(GetAvatarActorFromActorInfo())->DemonicSoul;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(DemonicSoul->GetActorLocation());
	SpawnTransform.SetRotation(DemonicSoul->GetActorQuat());
	AHauntProjectile* HauntProjectile = GetWorld()->SpawnActorDeferred<AHauntProjectile>(
		HauntProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(), 
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// Copying the Projectile Movement settings for BP_FireBall, since we want the "return" version of the projectile to travel ("Lerp") from the Demonic Soul's Location back to the Vessel without any obstruction/collision
	HauntProjectile->ProjectileMovement->SetComponentTickEnabled(false);
	HauntProjectile->ProjectileMovement->InitialSpeed = 0.f;
	HauntProjectile->ProjectileMovement->MaxSpeed = 0.f;
	HauntProjectile->ProjectileMovement->bAutoActivate = false;
	HauntProjectile->SetActorEnableCollision(false); // Purely cosmetic projectile

	FGameplayCueParameters CueParams;
	CueParams.Location = DemonicSoul->GetActorLocation();
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(HauntProjectile, FAuraGameplayTags::Get().GameplayCue_Rift_Haunt, CueParams);

	HauntProjectile->ReturnToActor = DemonicSoul->OwnerActor;
	UAuraAbilitySystemLibrary::HideSummon(DemonicSoul, true);
	HauntProjectile->FinishSpawning(SpawnTransform);
	HauntProjectile->StartReturnTimeline();
}

void UHaunt::FireDemonicSoul(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AHauntProjectile* HauntProjectile = GetWorld()->SpawnActorDeferred<AHauntProjectile>(
		HauntProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	HauntProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	HauntProjectile->FinishSpawning(SpawnTransform);
}