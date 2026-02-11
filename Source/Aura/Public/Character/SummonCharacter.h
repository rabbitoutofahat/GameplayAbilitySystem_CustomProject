// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacterBase.h"
#include "Interaction/SummonInterface.h"
#include "SummonCharacter.generated.h"

class UAuraUserWidget;
class UAuraGameplayAbility;

DECLARE_DYNAMIC_DELEGATE(FOnSummonDeathSignature);

/**
 * 
 */
UCLASS()
class AURA_API ASummonCharacter : public AAICharacterBase, public ISummonInterface
{
	GENERATED_BODY()

public:
	virtual void PossessedBy(AController* NewController) override;

	/* Combat Interface */
	virtual void Die(const FVector& DeathImpulse) override;
	/* end Combat Interface */

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerActor; // For some reason setting both the Owner and the OwnerActor Blackboard Key to GetOwner() doesn't work, so we set our own OwnerActor variable

	// TODO: Move these to a DemonicSoul Subclass
	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingSpawned = false; // Controls when to switch to the Spawn Animation State in the AnimBP, used for "respawning" the Demonic Soul after Haunt ends

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxEnergyChanged;

	UPROPERTY(BlueprintReadOnly)
	bool bFullEnergy = false; // Used to cancel channeled abilities when Special Attack is ready

	UFUNCTION(BlueprintCallable)
	void ShouldEnableSpecial(bool bEnable); // Used to change the ShouldUseSpecial Blackboard Key

	float Lifespan = 0.f; // Set by the AuraAttributeSet Lifespan Attribute, used to determine how long the summon should live before automatically dying

	FOnSummonDeathSignature OnSummonDeathDelegate;

protected:
	virtual void BeginPlay() override;

	virtual void BindCallbacksToDependencies(const UAuraAttributeSet* AuraAS) override;
	virtual void BroadcastInitialValues(const UAuraAttributeSet* AuraAS) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EnergyOnHit = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float SummonCost;
};
