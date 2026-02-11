// Roland Frances


#include "Character/PlayableClasses/VesselSummons/DemonicSoul.h"
#include "UI/Widget/AuraUserWidget.h"

void ADemonicSoul::BeginPlay()
{
	Super::BeginPlay();
	bIsDemonicSoul = true;

	if (HealthFrameClass != nullptr)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthFrameClass);
		HealthFrame = Cast<UAuraUserWidget>(Widget);
		HealthFrame->SetAnchorsInViewport(FAnchors(0.0, 0.5)); // Middle-Left Screen Anchor
		HealthFrame->SetAlignmentInViewport(FVector2D(-0.5, 1.5)); // Right and Up from the Anchor
		HealthFrame->AddToViewport();
		HealthFrame->SetWidgetController(this);
	}
}
