// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacter.h"
#include "Character/SummonCharacter.h"
#include "Vessel.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AVessel : public AAuraCharacter
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	ASummonCharacter* SpawnSummonedMinion(UClass* Class, const FTransform& SpawnTransform, int32 OwnerLevel);

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASummonCharacter> DemonicSoul; // Set in GA_DemonicSoul

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASummonCharacter> DemonicSoulClass;

	/*
    * Lesser Demons can be spawned in multiple ways via upgrades on different abilities, so we set a reference here as we need to access the Vessel 
	* class to check for the appropriate upgrade tags anyway
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASummonCharacter> LesserDemonClass;
};
