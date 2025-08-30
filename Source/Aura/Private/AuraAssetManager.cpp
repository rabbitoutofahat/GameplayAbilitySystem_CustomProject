// Copyright Druid Mechanics


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitialiseNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData(); // Does a lot of things, in particular it initialises the target data script struct cache for FAbilityTargetData
}
