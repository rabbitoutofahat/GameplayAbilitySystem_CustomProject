// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Haunt.h"
#include "Actor/Vessel/HauntProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AI/AuraAIController.h"
#include "BrainComponent.h"
#include "Character/SummonCharacter.h"

void UHaunt::HideSummon(ASummonCharacter* SummonClass, bool Enable)
{
	SummonClass->SetActorHiddenInGame(Enable);
	SummonClass->SetActorEnableCollision(!Enable);
	SummonClass->SetActorTickEnabled(!Enable);
	SummonClass->IsValidTarget = !Enable;
	if (Enable) Cast<AAuraAIController>(SummonClass->GetController())->GetBrainComponent()->StopLogic(FString("Hide Summon Actor"));
}

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

	HauntProjectile->ReturnToActor = DemonicSoul->OwnerActor;
	HideSummon(DemonicSoul, true);
	HauntProjectile->FinishSpawning(SpawnTransform);
	HauntProjectile->StartReturnTimeline();
}
