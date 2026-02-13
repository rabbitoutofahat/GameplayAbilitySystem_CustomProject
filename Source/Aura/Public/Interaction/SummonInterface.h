// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SummonInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSummonDeathSignature);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
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
	virtual FOnSummonDeathSignature& GetOnSummonDeathDelegate() = 0;
};
