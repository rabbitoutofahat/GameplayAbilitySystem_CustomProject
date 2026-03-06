// Roland Frances

#pragma once

#include "CoreMinimal.h"
#include "Character/SummonCharacter.h"
#include "Dregling.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ADregling : public ASummonCharacter
{
	GENERATED_BODY()
	
public:
	// Placed on the Dregling Summon Character Class as there are multiple ways to apply this debuff
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrades")
	TSubclassOf<UGameplayEffect> MoltenTouchDebuffClass;

};
