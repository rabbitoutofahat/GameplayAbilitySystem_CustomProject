// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Haunt.generated.h"

class ASummonCharacter;
class AHauntProjectile;

/**
 * 
 */
UCLASS()
class AURA_API UHaunt : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASummonCharacter> DemonicSoulClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHauntProjectile> HauntProjectileClass;
};
