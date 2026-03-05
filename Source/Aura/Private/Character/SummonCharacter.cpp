// Roland Frances


#include "Character/SummonCharacter.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/PlayableClasses/Vessel.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Vessel/SoulSiphon.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

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

	if (!IsA(ADemonicSoul::StaticClass()) &&
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor)->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Passive_DemonicSoul_DeathRattle))
	{
		AVessel* Vessel = Cast<AVessel>(OwnerActor);
		Vessel->DemonicSoul->GetOnSummonDeathDelegate().Broadcast(); // Need to broadcast specifically to the Demonic Soul's Delegate
	}

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

FOnSummonDeathSignature& ASummonCharacter::GetOnSummonDeathDelegate()
{
	return OnSummonDeathDelegate;
}

void ASummonCharacter::ShouldEnableSpecial(bool bEnable)
{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("ShouldUseSpecial"), bEnable);
	bFullEnergy = bEnable;
}

void ASummonCharacter::AddLifespan(float InLifespan)
{
	if (Lifespan <= 0.f) return; // Avoid executing this function on spawn before Lifespan is set by the Attribute Set
	AdditionalLifespan += InLifespan - Lifespan;
	Lifespan = InLifespan;
}

void ASummonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SummonFrameClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SummonFrameClass);
		SummonFrame = Cast<UAuraUserWidget>(Widget);
		SummonFrame->SetAnchorsInViewport(FAnchors(0.0, 0.5)); // Middle-Left Screen Anchor
		SummonFrame->SetAlignmentInViewport(FVector2D(-0.5, 1.5)); // Right and Up from the Anchor
		SummonFrame->AddToViewport();
		SummonFrame->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		BindCallbacksToDependencies(AuraAS);
		BroadcastInitialValues(AuraAS);
		BindHitReactTagChangeDelegate();

		Lifespan = AuraAS->GetLifespan();
		// Callback function has an input parameter so we need to bind a lambda. If a summon reaches its full lifespan, no death impulse is needed
		LifespanDelegate.BindLambda([this]() {OnLifespanTimerEnd(); });
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
			else ShouldEnableSpecial(false);
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

void ASummonCharacter::OnLifespanTimerEnd()
{
	if (AdditionalLifespan > 0.1f)
	{
		GetWorldTimerManager().ClearTimer(LifespanTimerHandle);
		GetWorldTimerManager().SetTimer(LifespanTimerHandle, LifespanDelegate, AdditionalLifespan, false);
		AdditionalLifespan = 0.f; // Reset AdditionalLifespan so that it doesn't interfere with future lifespan changes
	}
	else Die(FVector::ZeroVector);
}