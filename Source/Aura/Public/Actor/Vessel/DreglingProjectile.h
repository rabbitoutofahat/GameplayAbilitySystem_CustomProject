// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "DreglingProjectile.generated.h"

class ASummonCharacter;
class AAuraEffectActor;

/**
 * 
 */
UCLASS()
class AURA_API ADreglingProjectile : public AAuraProjectile
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void PlayImpactEffects() override;

	// Used to differentiate between the main Dregling Projectile and secondary Lesser Demon Projectiles that come with the Kinetic Bombardment Upgrade
	UPROPERTY(EditDefaultsOnly)
	bool bIsPrimaryProjectile = true;

private:
	void SpawnFlamePatch(int32 Level); // Spawn a flame patch effect actor on the ground, whose damage scales with the effect actor's level

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASummonCharacter> ClassToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	TSubclassOf<AAuraEffectActor> FlamePatchClass;
};
