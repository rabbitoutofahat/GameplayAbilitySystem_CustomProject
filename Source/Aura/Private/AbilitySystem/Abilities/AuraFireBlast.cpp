// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>FireBolt</>\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n" 
		"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

		// Description
		"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, causing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>"),
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>Next Level</>\n"

		// Details
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

		// Description
		"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, causing </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>"),
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage);
}
