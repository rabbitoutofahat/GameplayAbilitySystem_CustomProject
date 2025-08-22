// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

/**
 *
 */
UCLASS(Abstract) // Abstract prevents this class base from being dragged into the level
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	// Pointers for our ability system component and attribute set after implementing AbilitySystemInterface above, helps everything in the system interact with eachother cleanly
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	// TObjectPtr is a pointer wrapper that, unlike raw pointers, lets you track how often the pointer is accessed/dereferenced and ensures assets are not loaded until needed or used (aka lazy loading), among other things
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/*
	* Where to call InitAbilityActorInfo for player controlled characters:
	* 1. ASC lives on the pawn -> PossessedBy() on the pawn on the server, AcknowledgePossesion() on the player controller on the client
	* 2. ASC lives on the player state -> PossessedBy() on the pawn on the server, OnRep_PlayerState() (i.e., via rep notify) on the player controller on the client
	*/
	virtual void InitAbilityActorInfo(); // Must be done after possession (the controller has been set for the Pawn)

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitialiseDefaultAttributes() const;
};
