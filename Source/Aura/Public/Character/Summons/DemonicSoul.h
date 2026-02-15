// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/SummonCharacter.h"
#include "DemonicSoul.generated.h"

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
};
