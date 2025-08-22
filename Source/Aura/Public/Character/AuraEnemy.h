// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

// As soon as we added the pure virtual functions Highlight/UnhighlightActor, this became an abstract class which cannot be instantiated without overriding
public:
	AAuraEnemy();

	/* Enemy Interface */
	virtual void HighlightActor() override; 
	virtual void UnHighlightActor() override;
	/* end Enemy Interface */

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* end Combat Interface */

protected:
	virtual void BeginPlay() override;

	/*
     * Where to call InitAbilityActorInfo for AI-controlled characters:
	 * 1. ASC lives on the pawn -> BeginPlay() on the pawn on the server and the client
     */ 
	virtual void InitAbilityActorInfo() override;
 	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
