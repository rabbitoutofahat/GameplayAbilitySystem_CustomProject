// Roland Frances


#include "AbilitySystem/Abilities/Vessel/Haunt.h"
#include "Character/SummonCharacter.h"
#include "Actor/Vessel/HauntProjectile.h"

void UHaunt::HideDemonicSoul(bool Enable)
{
	ASummonCharacter* DemonicSoul = Cast<ASummonCharacter>(DemonicSoulClass);
	DemonicSoul->SetActorHiddenInGame(Enable);
	DemonicSoul->SetActorEnableCollision(!Enable);
	DemonicSoul->SetActorTickEnabled(!Enable);
}

AHauntProjectile* UHaunt::SpawnReturnProjectile()
{
	ASummonCharacter* DemonicSoul = Cast<ASummonCharacter>(DemonicSoulClass);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(DemonicSoul->GetActorLocation());
	SpawnTransform.SetRotation(DemonicSoul->GetActorQuat());
	AHauntProjectile* HauntProjectile = GetWorld()->SpawnActorDeferred<AHauntProjectile>(
		HauntProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(), 
		CurrentActorInfo->PlayerController->GetPawn(),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	HauntProjectile->ReturnToActor = DemonicSoul->OwnerActor;
	HauntProjectile->SetActorEnableCollision(false); // Purely cosmetic projectile
	HideDemonicSoul(true);
	HauntProjectile->FinishSpawning(SpawnTransform);
	return HauntProjectile;
}
