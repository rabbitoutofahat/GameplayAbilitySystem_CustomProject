// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32 /*StatValue*/);

/**
 * Acts as the 'Owner' Actor (or in the context of Gameplay Effects, the 'Instigator') for our Aura player character, as this is the owner of the Ability System Component
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Pointers for our ability system component and attribute set after implementing AbilitySystemInterface above, helps everything in the system interact with eachother cleanly
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // Pure virtual function of IAbilitySystemInterface from AbilitySystemInterface.h
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo; // Keeping this on the Player State instead of a Widget Controller as Player States exist on both the clients and server

	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
	FOnPlayerStatChangedSignature OnXPChangedDelegate;
	FOnPlayerStatChangedSignature OnPlayerAttributePointChangedDelegate;
	FOnPlayerStatChangedSignature OnPlayerSpellPointChangedDelegate;

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	void SetLevel(int32 InLevel);
	void SetXP(int32 InXP);

	void AddToLevel(int32 InLevel);
	void AddToXP(int32 InXP);
	void AddToAttributePoints(int32 InPoints);
	void AddToSpellPoints(int32 InPoints);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;

	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints) const;
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints) const;
};
