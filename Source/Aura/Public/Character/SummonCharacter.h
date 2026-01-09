// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacterBase.h"
#include "Interaction/SummonInterface.h"
#include "SummonCharacter.generated.h"

class UAuraUserWidget;
class UAuraGameplayAbility;

/**
 * 
 */
UCLASS()
class AURA_API ASummonCharacter : public AAICharacterBase, public ISummonInterface
{
	GENERATED_BODY()

public:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor; // For some reason setting owner and setting the OwnerActor Blackboard Key to GetOwner() doesn't work, so we set our own owner variable

	// TODO: Move these to a DemonicSoul Subclass
	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingSpawned = false; // Controls when to switch to the Spawn Animation State in the AnimBP, used for "respawning" the Demonic Soul after Haunt ends

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxEnergyChanged;

	UPROPERTY(BlueprintReadOnly)
	bool bFullEnergy = false; // Used to cancel channeled abilities when Special Attack is ready

protected:
	virtual void BeginPlay() override;

	virtual void BindCallbacksToDependencies(const UAuraAttributeSet* AuraAS) override;
	virtual void BroadcastInitialValues(const UAuraAttributeSet* AuraAS) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EnergyOnHit = 20.f;

	// TODO: Move these to a DemonicSoul Subclass
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> HealthFrame;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> HealthFrameClass;

	UFUNCTION(BlueprintCallable)
	void ShouldEnableSpecial(bool bEnable); // Used to change the ShouldUseSpecial Blackboard Key
};
