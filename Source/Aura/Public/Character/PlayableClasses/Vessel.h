// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacter.h"
#include "Character/SummonCharacter.h"
#include "Character/Summons/DemonicSoul.h"
#include "Vessel.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AVessel : public AAuraCharacter
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	ASummonCharacter* SpawnSummonedMinion(UClass* Class, const FTransform& SpawnTransform, int32 OwnerLevel);

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ADemonicSoul> DemonicSoul; // Set in GA_DemonicSoul

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ADemonicSoul> DemonicSoulClass;

	/*
    * Lesser Demons can be spawned in multiple ways via upgrades on different abilities, so we set a reference here as we need to access the Vessel 
	* class to check for the appropriate upgrade tags anyway
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASummonCharacter> LesserDemonClass;

	/*
	* Check for the Abyssal Dominion upgrade tag when summoning Demons. If present, apply the buff to the demon being summoned.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	TSubclassOf<UGameplayEffect> AbyssalDominionBuffClass;

	/*
	* Check for the Portent of Destruction upgrade tag when using abilities that open rifts. If present, apply the buff to the Vessel, making the next Summon Dregling cast free
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	TSubclassOf<UGameplayEffect> PortentOfDestructionBuffClass;

	/*
	* Cooldown variables for the Kinetic Bombardment upgrade for Summon Dregling stored here since the cooldown needs to be tracked across multiple uses of the ability.
	* Set true by default so we don't have to worry about turning it on when the player gets the Kinetic Bombardment upgrade.
	*/
	bool bKineticBombardmentOffCD = true;
	FTimerHandle KineticBombardmentTimerHandle;
};
