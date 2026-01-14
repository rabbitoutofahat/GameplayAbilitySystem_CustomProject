// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilityTypes.h"
#include "ProjectileSpawner.generated.h"

class AAuraProjectile;

UCLASS()
class AURA_API AProjectileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileSpawner();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Lifespan = 8.f;

};
