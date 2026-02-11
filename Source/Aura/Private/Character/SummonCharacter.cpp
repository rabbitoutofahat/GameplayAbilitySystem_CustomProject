// Roland Frances


#include "Character/SummonCharacter.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Vessel/SoulSiphon.h"
#include "UI/Widget/AuraUserWidget.h"

void ASummonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// OwnerActor should hopefully be set upon a SummonCharacter Blueprint being created in the world
	AuraAIController->GetBlackboardComponent()->SetValueAsObject(FName("OwnerActor"), OwnerActor);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("ShouldUseSpecial"), false);
}

void ASummonCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	if (!bIsDemonicSoul) OnSummonDeathDelegate.Broadcast();

	// If the owner is playing as Vessel and has the Soul Siphon Reclamation ability, refund mana on death based on the SummonCost
	AVessel* Vessel = Cast<AVessel>(OwnerActor);
	if (Vessel && Vessel->GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon_Reclamation))
	{
		UAuraAttributeSet* VesselAS = Cast<UAuraAttributeSet>(Vessel->GetAttributeSet());
		UAuraAbilitySystemComponent* VesselASC = Cast<UAuraAbilitySystemComponent>(Vessel->GetAbilitySystemComponent());
		USoulSiphon* SoulSiphonAbility = Cast<USoulSiphon>(VesselASC->GetAbilitySpecFromTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon)->Ability);
		VesselAS->SetMana(VesselAS->GetMana() + (SummonCost * SoulSiphonAbility->ReclamationPercentage));
		// TODO: Check if dead summon is Demonic Soul, if so restore some health
	}
}

void ASummonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthFrameClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthFrameClass);
		HealthFrame = Cast<UAuraUserWidget>(Widget);
		HealthFrame->SetAnchorsInViewport(FAnchors(0.0, 0.5)); // Middle-Left Screen Anchor
		HealthFrame->SetAlignmentInViewport(FVector2D(-0.5, 1.5)); // Right and Up from the Anchor
		HealthFrame->AddToViewport();
		HealthFrame->SetWidgetController(this);
		bIsDemonicSoul = true; // Only the Demonic Soul has a Health Frame widget
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		BindCallbacksToDependencies(AuraAS);
		BroadcastInitialValues(AuraAS);
		BindHitReactTagChangeDelegate();
		
		Lifespan = AuraAS->GetLifespan();
		FTimerDelegate LifespanDelegate;
		// Callback function has an input parameter so we need to bind a lambda. If a summon reaches its full lifespan, no death impulse is needed
		LifespanDelegate.BindLambda([this]() {Die(FVector::ZeroVector); }); 
		FTimerHandle LifespanTimerHandle;
		GetWorldTimerManager().SetTimer(LifespanTimerHandle, LifespanDelegate, Lifespan, false);
	}
}

void ASummonCharacter::BindCallbacksToDependencies(const UAuraAttributeSet* AuraAS)
{
	Super::BindCallbacksToDependencies(AuraAS);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetEnergyAttribute()).AddLambda(
		[this, AuraAS](const FOnAttributeChangeData& Data)
		{
			OnEnergyChanged.Broadcast(Data.NewValue);
			if (Data.NewValue >= AuraAS->GetMaxEnergy()) ShouldEnableSpecial(true);
			else ShouldEnableSpecial(false); // Should we disable here or handle in the Gameplay Ability?
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxEnergyAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxEnergyChanged.Broadcast(Data.NewValue);
		}
	);
}

void ASummonCharacter::BroadcastInitialValues(const UAuraAttributeSet* AuraAS) const
{
	Super::BroadcastInitialValues(AuraAS);
	OnEnergyChanged.Broadcast(AuraAS->GetEnergy());
	OnMaxEnergyChanged.Broadcast(AuraAS->GetMaxEnergy());
}

void ASummonCharacter::ShouldEnableSpecial(bool bEnable)
{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("ShouldUseSpecial"), bEnable);
	bFullEnergy = bEnable;
}