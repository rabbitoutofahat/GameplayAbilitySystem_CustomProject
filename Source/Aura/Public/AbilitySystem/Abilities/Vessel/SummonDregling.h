// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "SummonDregling.generated.h"

class AAuraProjectile;
class AVessel;
class UAuraAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class AURA_API USummonDregling : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	static float GetPortentProcChance(UAuraAbilitySystemComponent* ASC);

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat AbilityCost; // As we use an MMC to calculate ability cost under certain conditions, store cost curve table here rather than on the cost gameplay effect associated with this ability

protected:
	// Summons a Dregling Summon Character from the sky that crashes down onto the TargetLocation. Should feel like a meteor strike, with XY-offsets to have it come down at an angle.
	UFUNCTION(BlueprintCallable)
	void SpawnDreglingProjectile(const FVector& TargetLocation, const float XOffset, const float YOffset);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnHeight;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> DreglingProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades|Portent Of Destruction")
	float ProcChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades|Kinetic Bombardment")
	TSubclassOf<AAuraProjectile> KineticBombardmentProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades|Kinetic Bombardment")
	int32 NumProjectiles = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades|Kinetic Bombardment")
	float ProjectileSpread = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Upgrades|Kinetic Bombardment")
	float Cooldown = 20.f;

	void SpawnKineticBombardmentProjectiles(const FVector& SpawnLocation, const FVector& TargetLocation);
	void SetKineticBombardmentActiveStatus(AVessel* Vessel, bool bInStatus);
};
