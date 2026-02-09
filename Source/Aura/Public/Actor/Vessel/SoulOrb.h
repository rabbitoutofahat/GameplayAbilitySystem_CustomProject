// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "SoulOrb.generated.h"

class UGameplayEffect;
class ASummonCharacter;

/**
 * 
 */
UCLASS()
class AURA_API ASoulOrb : public AAuraProjectile
{
	GENERATED_BODY()
	
public:
	float VengefulSpiritSpawnPercent;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> SoulOrbPassiveEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Vengeful Manifestation Upgrade")
	TSubclassOf<ASummonCharacter> VengefulSpiritClass;

	// Vengeful Spirit spawn properties are stored here rather than on the Soul Siphon GA as these aren't intended to be tuning parameters like the spawn proc chance
	UPROPERTY(EditDefaultsOnly, Category = "Vengeful Manifestation Upgrade")
	float SpiritSpawnSpread = 180.f;

	UPROPERTY(EditDefaultsOnly, Category = "Vengeful Manifestation Upgrade")
	float SpiritSpawnDistanceMin = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Vengeful Manifestation Upgrade")
	float SpiritSpawnDistanceMax = 300.f;
};
