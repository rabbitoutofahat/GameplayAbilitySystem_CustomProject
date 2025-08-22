// Copyright Druid Mechanics


#include "Player/AuraPlayerState.h"
#include <Net/UnrealNetwork.h>

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // Gameplay effects are replicated to the owning client only. Used for multiplayer player-controlled actors

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f; // How often the server will try to update clients
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
}

