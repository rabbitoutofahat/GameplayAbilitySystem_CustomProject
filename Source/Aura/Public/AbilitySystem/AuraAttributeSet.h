// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

// For using the ATTRIBUTE_ACCESSORS macro, which defines a set of helper functions for accessing and initialising attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {} // Default constructor (takes no arguments and does nothing)

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

};

/*
* TBaseStaticDelegateInstance is a template function delegate that binds a function pointer which takes 0 arguments but returns a value type which we define as the first template argument. 
* To extend delegates, we implement a policy struct and pass it as the second template argument (see default policy definition).
* In this case, we've wrapped TBaseStaticDelegateInstance in a true template function pointer capable of storing the address of a function of any function signature that we choose.
*   
*   TStaticFuncPtr<float(int32, float, int32)> RandomFunctionPointer;
*	static float RandomFunction(int32 I, float F, int32 I2); { return 0.f; }
*
*	(In .cpp)
*   RandomFunctionPointer = RandomFunction();
*   float F = RandomFunctionPointer(0, 0.f, 0);
* 
*  typedef is specific to the FGameplayAttribute() signature, but TStaticFuncPtr is generic to any signature chosen.
*/
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	/**
	* When creating a replicated attribute, we must do the following:
	* 1. Add the ReplicatedUsing specifier and assign a RepNotify
	* 2. Register the variable for replication in GetLifetimeReplicatedProps
	* 3. Notify the Ability System of the replication with GAMEPLAYATTRIBUTE_REPNOTIFY (see .cpp file)
	*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Method for clamping
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/*
	* By mapping our Gameplay Tags to static function pointers of type FGameplayAttributes, we can hold static getter functions for each Gameplay Attribute in these function 
	* pointers and skip the usual process of declaring our own delegate signature and binding functions to it.
	*/
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/*
    * Primary Attributes
    */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	/*
     * Secondary Attributes
     */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armour, Category = "Secondary Attributes")
	FGameplayAttributeData Armour;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armour);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmourPierce, Category = "Secondary Attributes")
	FGameplayAttributeData ArmourPierce;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmourPierce);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritChance, Category = "Secondary Attributes")
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritRes, Category = "Secondary Attributes")
	FGameplayAttributeData CritRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_Armour(const FGameplayAttributeData& OldArmour) const;

	UFUNCTION()
	void OnRep_ArmourPierce(const FGameplayAttributeData& OldArmourPierce) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const;

	UFUNCTION()
	void OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const;
	
	UFUNCTION()
	void OnRep_CritRes(const FGameplayAttributeData& OldCritRes) const;

	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;

	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	/*
	* Damage Resistances
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireRes, Category = "Damage Resistances")
	FGameplayAttributeData FireRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningRes, Category = "Damage Resistances")
	FGameplayAttributeData LightningRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneRes, Category = "Damage Resistances")
	FGameplayAttributeData ArcaneRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalRes, Category = "Damage Resistances")
	FGameplayAttributeData PhysicalRes;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalRes);

	UFUNCTION()
	void OnRep_FireRes(const FGameplayAttributeData& OldFireRes) const;

	UFUNCTION()
	void OnRep_LightningRes(const FGameplayAttributeData& OldLightningRes) const;

	UFUNCTION()
	void OnRep_ArcaneRes(const FGameplayAttributeData& OldArcaneRes) const;

	UFUNCTION()
	void OnRep_PhysicalRes(const FGameplayAttributeData& OldPhysicalRes) const;

	/*
	* Vital Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes") // Whenever an attribute is replicated down to the client, the client gets a rep notify, in this case called "OnRep_Health"
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	/*
	* Meta Attributes (not replicated, we set and process them on the server and then change any affected attributes based on those calculations)
	*/
	UPROPERTY(BluePrintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage; 
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BluePrintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void SendXPEvent(const FEffectProperties& Props);
};
