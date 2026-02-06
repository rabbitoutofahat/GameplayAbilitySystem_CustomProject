// Roland Frances


#include "Character/SummonCharacter.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Vessel/SoulSiphon.h"

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
	
	// If the owner is playing as Vessel and has the Soul Siphon Reclamation ability, refund mana on death based on the SummonCost
	if (Cast<AVessel>(OwnerActor) && GetAbilitySystemComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon_Reclamation))
	{
		UAuraAttributeSet* OwnerAS = Cast<UAuraAttributeSet>(Cast<AAuraCharacterBase>(OwnerActor)->GetAttributeSet());
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(Cast<AAuraCharacterBase>(OwnerActor)->GetAbilitySystemComponent());
		USoulSiphon* SoulSiphonAbility = Cast<USoulSiphon>(AuraASC->GetAbilitySpecFromTag(FAuraGameplayTags::Get().Abilities_Vessel_SoulSiphon)->Ability);
		OwnerAS->SetMana(OwnerAS->GetMana() + (SummonCost * SoulSiphonAbility->ReclamationPercentage));
		// TODO: Check if dead summon is Demonic Soul, if so restore some health
	}
}

void ASummonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthFrameClass != nullptr)
	{
		// TODO: Move to DemonicSoul / Major Summon sub-class, don't need for Minor Summons
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthFrameClass);
		HealthFrame = Cast<UAuraUserWidget>(Widget);
		HealthFrame->SetAnchorsInViewport(FAnchors(0.0, 0.5)); // Middle-Left Screen Anchor
		HealthFrame->SetAlignmentInViewport(FVector2D(-0.5, 1.5)); // Right and Up from the Anchor
		HealthFrame->AddToViewport();
		HealthFrame->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		BindCallbacksToDependencies(AuraAS);
		BroadcastInitialValues(AuraAS);
		BindHitReactTagChangeDelegate();
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