// Roland Frances


#include "Character/Summons/DemonicSoul.h"
#include "AbilitySystem/Abilities/Vessel/Haunt.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"

void ADemonicSoul::Die(const FVector& DeathImpulse)
{
	/*
	* On death destroy the Demonic Soul, spawn a "return" Haunt projectile, and start a respawn timer.
	* When the timer finishes, the Demonic Soul should be respawned at the Vessel's location.
	* TODO: Bind to OnSummonDeath Event on Demonic Soul's Summon Frame widget, change the tint and display the respawn timer countdown
	*/
	Super::Die(DeathImpulse);

	//TODO: Add special effect when Demonic Soul dies, "retreating" back to the Vessel
	//GetAvatarActorFromActorInfo() returns nullptr
	//AVessel* Vessel = Cast<AVessel>(OwnerActor);
	//UAuraAbilitySystemComponent* VesselASC = Cast<UAuraAbilitySystemComponent>(Vessel->GetAbilitySystemComponent());
	//UHaunt* HauntAbility = Cast<UHaunt>(VesselASC->GetAbilitySpecFromTag(FAuraGameplayTags::Get().Abilities_Utility_Haunt)->Ability);
	//HauntAbility->SpawnReturnProjectile();

	//TODO: If the player has the Hellforged Reconstitution Upgrade, create a radial Niagara Effect indicating the effective range of the respawn explosion

	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Buff_HellforgedReconstitution)) // TODO: Once more floors are implemented, grant another buff tag on floor change
	{
		RespawnTimer = HellforgedReconstitutionRespawnTimer;
		FGameplayTagContainer EffectsToRemove;
		EffectsToRemove.AddTag(FAuraGameplayTags::Get().Buff_HellforgedReconstitution);
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(EffectsToRemove);
	}
	
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ADemonicSoul::OnRespawnTimerEnd, RespawnTimer, false);
	RespawnTimerStartDelegate.Broadcast();
}

void ADemonicSoul::OnRespawnTimerEnd()
{
	AVessel* Vessel = Cast<AVessel>(OwnerActor);
	FHitResult Hit;
	FVector SpawnLocation;
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + LineTraceStart, GetActorLocation() + LineTraceEnd, ECollisionChannel::ECC_Visibility);
	if (Hit.bBlockingHit) SpawnLocation = Hit.ImpactPoint + SpawnLocationZOffset;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());
	ASummonCharacter* Summon = Vessel->SpawnSummonedMinion(Vessel->DemonicSoulClass, SpawnTransform, Execute_GetPlayerLevel(Vessel));
	Vessel->DemonicSoul = Cast<ADemonicSoul>(Summon); // Set the newly respawned Demonic Soul as the Vessel's current Demonic Soul reference

	if (Vessel && Summon)
	{
		if (Vessel->GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_DemonicSoul_HellforgedReconstitution))
		{
			CreateExplosionOnRevival();
		}
		if (Vessel->GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_DemonicSoul_FriendsInLowPlaces))
		{
			TArray<FVector> LesserDemonSpawnLocations = UAuraAbilitySystemLibrary::EvenlyRotatedVectors(Summon->GetActorForwardVector(), FVector::UpVector, SpawnSpread, NumDemonsSpawned);
			for (const FVector& LesserDemonSpawn : LesserDemonSpawnLocations)
			{
				FTransform LesserDemonSpawnTransform;
				LesserDemonSpawnTransform.SetLocation(Summon->GetActorLocation() + (LesserDemonSpawn * LesserDemonSpawnDistance) + SpawnLocationZOffset);
				Vessel->SpawnSummonedMinion(Vessel->LesserDemonClass, LesserDemonSpawnTransform, Execute_GetPlayerLevel(Vessel));
			}
		}
	}
}

void ADemonicSoul::CreateExplosionOnRevival()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	TArray<AActor*> ActorsToDamage;
	FDamageEffectParams DamageEffectParams = MakeReviveExplosionDamageEffectParams();
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(DamageEffectParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, DamageEffectParams.RadialDamageOuterRadius, DamageEffectParams.RadialDamageOrigin);

	for (AActor* Actor : ActorsToDamage)
	{
		if (!Actor->ActorHasTag(FName("Enemy"))) continue;
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
		{
			FVector Direction = Actor->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.f;
			Direction.Normalize();
			const FVector ToTarget = Direction + FVector::UpVector * FMath::Sin(FMath::DegreesToRadians(KnockbackPitch));

			DamageEffectParams.Knockback = ToTarget * DamageEffectParams.KnockbackMagnitude;
			DamageEffectParams.DeathImpulse = ToTarget * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
		}
	}

	FGameplayCueParameters CueParams;
	CueParams.Location = DamageEffectParams.RadialDamageOrigin;
	UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_ReviveExplosion, CueParams);
}

FDamageEffectParams ADemonicSoul::MakeReviveExplosionDamageEffectParams()
{
	FDamageEffectParams Params;
	Params.WorldContextObject = this;
	Params.DamageGameplayEffectClass = ReviveExplosionDamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponent();
	Params.BaseDamage = ExplosionBaseDamage;
	Params.AbilityLevel = 1.f;
	Params.DamageType = FAuraGameplayTags::Get().Damage_Shadow;
	Params.DebuffChance = 0.f;
	Params.DebuffDamage = 0.f;
	Params.DebuffDuration = 0.f;
	Params.DebuffFrequency = 0.f;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackMagnitude = KnockbackMagnitude;
	Params.KnockbackChance = 100.f;

	Params.bIsRadialDamage = true;
	Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
	Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	Params.RadialDamageOrigin = GetActorLocation();

	return Params;
}
