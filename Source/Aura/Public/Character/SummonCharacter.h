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

protected:
	virtual void BeginPlay() override;

	// TODO: Move these to a DemonicSoul Subclass
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> HealthFrame;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> HealthFrameClass;
};
