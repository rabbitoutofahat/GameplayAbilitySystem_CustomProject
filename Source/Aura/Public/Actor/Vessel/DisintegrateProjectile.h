// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "DisintegrateProjectile.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class AURA_API ADisintegrateProjectile : public AAuraProjectile
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DisintegrateEffectClass;
};
