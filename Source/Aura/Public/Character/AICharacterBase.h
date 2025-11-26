// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h" // For using the FOnAttributeChangedSignature delegate
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AuraAIController.h"
#include "AICharacterBase.generated.h"

class UAuraAttributeSet;
class UBehaviorTree;

/**
 * 
 */
UCLASS(Abstract)
class AURA_API AAICharacterBase : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAICharacterBase();
	virtual void PossessedBy(AController* NewController) override;

	/* Combat Interface */
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual float GetAttackRadius_Implementation() const override;
	/* end Combat Interface */

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackRadius = 45.f;

	void SetLevel(int32 InLevel) { Level = InLevel; }

protected:
	virtual void BeginPlay() override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	void BindCallbacksToDependencies(const UAuraAttributeSet* AuraAS);
	void BroadcastInitialValues(const UAuraAttributeSet* AuraAS) const;
	void BindHitReactTagChangeDelegate();

	/*
	 * Where to call InitAbilityActorInfo for AI-controlled characters:
	 * 1. ASC lives on the pawn -> BeginPlay() on the pawn on the server and the client
	 */
	virtual void InitAbilityActorInfo() override;
	virtual void InitialiseDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
