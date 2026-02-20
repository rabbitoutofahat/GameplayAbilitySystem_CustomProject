// Roland Frances


#include "Character/Summons/DemonicSoul.h"
#include "AbilitySystem/Abilities/Vessel/Haunt.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/AuraUserWidget.h"

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
	// 
	//AVessel* Vessel = Cast<AVessel>(OwnerActor);
	//UAuraAbilitySystemComponent* VesselASC = Cast<UAuraAbilitySystemComponent>(Vessel->GetAbilitySystemComponent());
	//UHaunt* HauntAbility = Cast<UHaunt>(VesselASC->GetAbilitySpecFromTag(FAuraGameplayTags::Get().Abilities_Utility_Haunt)->Ability);
	//HauntAbility->SpawnReturnProjectile();

	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Buff_HellforgedReconstitution))
	{
		RespawnTimer = 3.f;
		FGameplayTagContainer EffectsToRemove;
		EffectsToRemove.AddTag(FAuraGameplayTags::Get().Buff_HellforgedReconstitution);
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(EffectsToRemove);
	}
	
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ADemonicSoul::Respawn, RespawnTimer, false);
	RespawnTimerStartDelegate.Broadcast();
}

void ADemonicSoul::Respawn()
{
	AVessel* Vessel = Cast<AVessel>(OwnerActor);
	FVector VesselLocation = Vessel->GetActorLocation();
	FHitResult Hit;
	FVector SpawnLocation;
	GetWorld()->LineTraceSingleByChannel(Hit, VesselLocation + LineTraceStart, VesselLocation + LineTraceEnd, ECollisionChannel::ECC_Visibility);
	if (Hit.bBlockingHit) SpawnLocation = Hit.ImpactPoint + SpawnLocationZOffset;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	ASummonCharacter* Summon = Vessel->SpawnSummonedMinion(Vessel->DemonicSoulClass, SpawnTransform, Execute_GetPlayerLevel(Vessel));
	Vessel->DemonicSoul = Cast<ADemonicSoul>(Summon); // Set the newly respawned Demonic Soul as the Vessel's current Demonic Soul reference
}
