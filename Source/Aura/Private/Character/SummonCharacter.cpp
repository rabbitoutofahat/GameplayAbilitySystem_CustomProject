// Roland Frances


#include "Character/SummonCharacter.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

ASummonCharacter::ASummonCharacter()
{
}

void ASummonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// OwnerActor should hopefully be set upon a SummonCharacter Blueprint being created in the world
	AuraAIController->GetBlackboardComponent()->SetValueAsObject(FName("OwnerActor"), OwnerActor);
}

void ASummonCharacter::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Move to DemonicSoul / Major Summon sub-class, don't need for Minor Summons
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthFrameClass);
	HealthFrame = Cast<UAuraUserWidget>(Widget);
	HealthFrame->SetAnchorsInViewport(FAnchors(0.0, 0.5)); // Middle-Left Screen Anchor
	HealthFrame->SetAlignmentInViewport(FVector2D(-0.5, 1.5)); // Right and Up from the Anchor
	HealthFrame->AddToViewport();
	HealthFrame->SetWidgetController(this);

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		BindCallbacksToDependencies(AuraAS);
		BroadcastInitialValues(AuraAS);
		BindHitReactTagChangeDelegate();
	}
}