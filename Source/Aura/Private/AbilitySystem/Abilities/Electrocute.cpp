// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Electrocute</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n" 
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Emits a beam of lightning at the target, repeatedly causing </>"
			"<Damage>%d </>"
			"<Default>lightning damage with a chance to stun at the end of the cast</>"),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	if (Level == 2)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Electrocute</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Emits a beam of lightning at the target that chains to an additional nearby target, repeatedly causing </>"
			"<Damage>%d </>"
			"<Default>lightning damage with a chance to stun at the end of the cast</>"),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Electrocute</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Emits a beam of lightning at the target that chains to %d additional nearby targets, repeatedly causing </>"
			"<Damage>%d </>"
			"<Default>lightning damage with a chance to stun at the end of the cast</>"),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level - 1, MaxNumShockTargets),
			ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 2)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Next Level</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Emits a beam of lightning at the target that chains to an additional nearby target, repeatedly causing </>"
			"<Damage>%d </>"
			"<Default>lightning damage with a chance to stun at the end of the cast</>"),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>Next Level</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Emits a beam of lightning at the target that chains to %d additional nearby targets, repeatedly causing </>"
			"<Damage>%d </>"
			"<Default>lightning damage with a chance to stun at the end of the cast</>"),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level - 1, MaxNumShockTargets),
			ScaledDamage);
	}
}