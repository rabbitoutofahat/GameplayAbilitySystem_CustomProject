// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FireBolt</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n" // %.1f displays one digit to the right of the decimal point
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
			"<Damage>%d </>"
			"<Default>fire damage with a chance to burn</>"),
			Level, 
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FireBolt</>\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f </><Small>sec</>\n\n"

			// Description
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d </>"
			"<Default>fire damage with a chance to burn</>"),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxProjectiles),
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
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
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
		"<Damage>%d </>"
		"<Default>fire damage with a chance to burn</>"),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxProjectiles),
		ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, bool bOverridePitch, float PitchOverride)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);
	const int32 NumProjectiles = FMath::Min(MaxProjectiles, GetAbilityLevel());

	if (NumProjectiles > 1)
	{
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0, 0, 10.f);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Direction * 75.f, 5.f, FLinearColor::Red, 120.f, 2.f);
		}
	}
	else
	{
		// Single projectile
		const FVector Start = SocketLocation + FVector(0, 0, 10.f);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Forward * 75.f, 5.f, FLinearColor::Red, 120.f, 2.f);
	}

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 1.f, FLinearColor::White, 120.f, 1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1.f, FLinearColor::White, 120.f, 1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1.f, FLinearColor::White, 120.f, 1.f);
}
