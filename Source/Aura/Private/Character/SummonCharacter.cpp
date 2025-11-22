// Roland Frances


#include "Character/SummonCharacter.h"


void ASummonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// OwningActor should hopefully be set upon a SummonCharacter Blueprint being created in the world
	if (GetOwner() != nullptr) AuraAIController->GetBlackboardComponent()->SetValueAsObject(FName("OwnerActor"), OwnerActor);
}

void ASummonCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ASummonCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}