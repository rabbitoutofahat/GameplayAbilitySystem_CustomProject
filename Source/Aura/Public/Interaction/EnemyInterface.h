// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

	/* 
	* Add interface functions to this class.This is the class that will be inherited to implement this interface.
	* Rather than casting to AuraEnemy and calling a function on that class, we have more flexibility with an interface, since any class can implement it and provide its own functionality.
	*/

public:
	virtual void HighlightActor() = 0; // By adding '=0' we make this a 'pure virtual function' (or 'abstract'), meaning we do not need to provide a definition
	virtual void UnHighlightActor() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) // BlueprintNativeEvents don't require a virtual keyword, instead you override it's Implementation on the desired class (in this case AAuraEnemy)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
