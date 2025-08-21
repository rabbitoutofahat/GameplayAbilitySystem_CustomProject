// Copyright Druid Mechanics


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); // A better way to get an ASC of any actor in blueprint than casting
	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);

	/* 
	* To apply a gameplay effect spec to self we need to make an gameplay effect spec handle, and the ability system component has the ability to do that. To make a spec handle, 
	* we in turn need to make an gameplay effect context handle.
	* 
	* These gameplay effect handles act as pointers that store the actual structs (such as spec and context) in a member variable called "Data", and has extra utilities that 
	* allow them to be polymorphic and replicate properly. It is often in GAS-related code that function names will often leave out the word 'handle', for example MakeOutgoingSpec()
	* really means MakeOutgoingSpecHandle().
	* 
	* The "Data" variable in these handles is itself another pointer wrapper (TSharedPtr). Below we use Get() to get the raw gameplay effect spec pointer, but as ApplyGameplayEffectSpectoSelf() 
	* only takes a reference to FGameplayEffectSpec, we deference (*) the pointer as the final step to extract the actual gameplay effect spec from its handle.
	*/
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext(); 
	EffectContextHandle.AddSourceObject(this); // "What object caused the effect?"
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite; // "Def" is the gameplay effect itself
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // If duration type is infinite and removed on end overlap, then store the active effect handle in a map
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC); // We now have a handle pair to keep track of so that we can remove the infinite gameplay effect in the removal policy
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // Check our map for a handle pair that has an ASC that matches the ASC of our end overlap actor
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1); // Removes 1 stack
				/*
				* If we want to remove a gameplay effect, we should also remove the pair from the map, but we are currently looping through the map itself.
				* Instead, as we move through gameplay effects, we can accumulate handle pairs we want to remove after we're done with the for loop in a container.
				*/
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

