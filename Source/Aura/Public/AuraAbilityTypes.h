#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	// Returns the actual struct used for serialization, subclasses must override this
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/*
	* Custom serialization required for network communication of ability types, subclasses must override this. When serializing, all the objects and variables in this struct become 'bit arrays' (strings of 1s and 0s). 
	* A Package Map takes these objects and maps them to and from indices, making it easier to determine where in the bit array one object ends and another begins.
	* The FArchive struct handles the saving, loading, and storing of serialized data. It overloads the << operator to serialize data into or from the archive.
	* Seriliazed data / bit arrays are an effective way of storing a series of boolean values, such as our GameplayEffectContext's properties (Instigator, EffectCauser, AbilityCDO, etc), 
	* plus some of our own Gameplay Attributes such as bIsBlockedHit and bIsCriticalHit. 
	*/
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};