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
};
