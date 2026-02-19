// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/SummonCharacter.h"
#include "DemonicSoul.generated.h"

class UHaunt;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RespawnTimer = 20.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHaunt> HauntAbilityClass;

	void Respawn();
};
