// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraEnemy.h"
#include "TowerSummonCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ATowerSummonCharacterBase : public AAuraEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> OwningActor;
};
