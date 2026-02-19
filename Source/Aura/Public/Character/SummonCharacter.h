// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacterBase.h"
#include "Interaction/SummonInterface.h"
#include "SummonCharacter.generated.h"

class UAuraUserWidget;
class UAuraGameplayAbility;
class UGameplayEffect;

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

	/* Summon Interface */
	virtual FOnSummonDeathSignature& GetOnSummonDeathDelegate() override;
	/* end Summon Interface */

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerActor; // For some reason setting both the Owner to GetOwner() doesn't work, so we set our own OwnerActor variable

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> SummonFrame;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> SummonFrameClass;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingSpawned = false; // Controls when to switch to the Spawn Animation State in the AnimBP

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxEnergyChanged;

	UPROPERTY(BlueprintReadOnly)
	bool bFullEnergy = false; // Used to cancel channeled abilities when Special Attack is ready

	UFUNCTION(BlueprintCallable)
	void ShouldEnableSpecial(bool bEnable); // Used to change the ShouldUseSpecial Blackboard Key

	float GetLifespan() { return Lifespan; }
	void SetLifespan(float InLifespan) { Lifespan = InLifespan; }

protected:
	virtual void BeginPlay() override;

	virtual void BindCallbacksToDependencies(const UAuraAttributeSet* AuraAS) override;
	virtual void BroadcastInitialValues(const UAuraAttributeSet* AuraAS) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EnergyOnHit = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float SummonCost; // Used to determine the amount of resources to refund to the player character under certain conditions, i.e., possessing the Reclamation upgrade gameplay tag

	UPROPERTY(BlueprintAssignable)
	FOnSummonDeathSignature OnSummonDeathDelegate; // Used for on-death effects, such as the Death Rattle upgrade buffing the Demonic Soul whenever a summon dies

private:
	/*
	* Set by the AuraAttributeSet Lifespan Attribute, used to determine how long the summon should live before automatically dying
	* Kept private as we don't want to expose this to the Demonic Soul class which can live indefinitely
	*/ 
	float Lifespan = 0.f;
};
