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

/**
 * 
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

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
