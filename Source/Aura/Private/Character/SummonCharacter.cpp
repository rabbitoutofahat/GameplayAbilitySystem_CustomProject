// Roland Frances


#include "Character/SummonCharacter.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Components/WidgetComponent.h"

ASummonCharacter::ASummonCharacter()
{
}

void ASummonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// OwnerActor should hopefully be set upon a SummonCharacter Blueprint being created in the world
	AuraAIController->GetBlackboardComponent()->SetValueAsObject(FName("OwnerActor"), OwnerActor);
}

void ASummonCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ASummonCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ASummonCharacter::BeginPlay()
{
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBarFrame->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	Super::BeginPlay();
}