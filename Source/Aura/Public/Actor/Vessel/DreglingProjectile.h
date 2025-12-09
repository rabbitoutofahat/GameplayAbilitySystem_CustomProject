// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "DreglingProjectile.generated.h"

class ASummonCharacter;

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

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASummonCharacter> DreglingClass;
};
