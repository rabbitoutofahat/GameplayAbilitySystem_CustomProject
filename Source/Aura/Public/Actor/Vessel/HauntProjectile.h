// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "HauntProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AHauntProjectile : public AAuraProjectile
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartReturnTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void PlayImpactEffects() override;

};
