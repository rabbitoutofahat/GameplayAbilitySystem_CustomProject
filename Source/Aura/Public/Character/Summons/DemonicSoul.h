// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/SummonCharacter.h"
#include "DemonicSoul.generated.h"

class UHaunt;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRespawnTimerStartSignature);

/**
 * 
 */
UCLASS()
class AURA_API ADemonicSoul : public ASummonCharacter
{
	GENERATED_BODY()
	
public:
	/* Combat Interface */
	virtual void Die(const FVector& DeathImpulse) override;
	/* end Combat Interface */	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RespawnTimer = 20.f;

	UPROPERTY(BlueprintAssignable)
	FRespawnTimerStartSignature RespawnTimerStartDelegate; // Start the respawn countdown timer for the Demonic Soul's Summon Frame Widget

	void Respawn();

private:
	FVector LineTraceStart = FVector(150.f, 0.f, 500.f);
	FVector LineTraceEnd = FVector(150.f, 0.f, -500.f);
	float SpawnLocationZOffset = 88.f; // To prevent the Demonic Soul from spawning inside the ground when respawning at the Vessel's location
};
