// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/SummonCharacter.h"
#include "DemonicSoul.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ADemonicSoul : public ASummonCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> HealthFrame;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> HealthFrameClass;
};
