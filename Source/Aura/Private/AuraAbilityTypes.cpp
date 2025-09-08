
#include "AuraAbilityTypes.h"

/*
* Start with an unsigned 8-bit integer (uint8) called RepBits, Each GameplayEffectContext property is represented by a single bit in the RepBits variable.
* 
* 1 << n means "1 shifted left by n bits", so 1 << 0 is 00000001, 1 << 1 is 00000010, 1 << 2 is 00000100, and so on. We compare RepBits with these values using the bitwise OR operator (|)
* in a set of if statements to determine which properties to serialize or deserialize. Comparing 2 sets of bits using bitwise OR means that if the bit in the n-th position for either set is 1,
* the n-th bit in the result will be 1, otherwise it will be 0.
* 
* For example, if we want to replicate the Instigator property, bReplicateInstigator will be true, and we set the 0th bit of RepBits to 1 using the bitwise OR operation (RepBits |= 1 << 0).
* In essence, the 0th bit of RepBits is 'flipped' to 1 if we want to replicate the Instigator property. If we have a Gameplay Ability that applies the Damage Gameplay Effect, and ExecCalc_Damage
* returns a blocked hit, then the 7th bit of RepBits will be 'flipped' to 1, indicating that bIsBlockedHit is true and should be serialized. Once RepBits is filled out, we serialize it using Ar.SerializeBits.
* 
* When storing the RepBits bit array into the archive, we use the bitwise AND operator (&) to 'read' the bit array (only if both the n-th bit of RepBits and 1 << n are 1 will the result be 1) and store
* the corresponding property into the archive.
*/


//, which is an unsigned 8-bit integer (uint8).
bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint8 RepBits = 0;

	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
	}

	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}