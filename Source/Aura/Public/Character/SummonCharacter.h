// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/AICharacterBase.h"
#include "Interaction/SummonInterface.h"
#include "SummonCharacter.generated.h"

class UAuraUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API ASummonCharacter : public AAICharacterBase, public ISummonInterface
{
	GENERATED_BODY()

public:
	ASummonCharacter();
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor; // For some reason setting owner and setting the OwnerActor Blackboard Key to GetOwner() doesn't work, so we set our own owner variable

	// TODO: Move these to a DemonicSoul Subclass
	UPROPERTY(BlueprintReadWrite)
	bool bIsBeingSpawned = false; // Controls when to switch to the Spawn Animation State in the AnimBP, used for "respawning" the Demonic Soul after Haunt ends

protected:
	virtual void BeginPlay() override;

	// TODO: Move these to a DemonicSoul Subclass
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> HealthFrame;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> HealthFrameClass;
};
