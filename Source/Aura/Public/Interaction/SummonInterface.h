// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SummonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USummonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ISummonInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) 
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void FollowOwner(AActor* InOwner);
};
