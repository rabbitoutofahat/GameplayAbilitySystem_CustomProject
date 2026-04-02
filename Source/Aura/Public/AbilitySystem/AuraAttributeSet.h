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
	UAuraAttributeSet* SourceAS = nullptr;

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
* typedef is specific to the FGameplayAttribute() signature, but TStaticFuncPtr is generic to any signature chosen.
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

	/*
	* When creating a replicated attribute, we must do the following:
	* 1. Add the ReplicatedUsing specifier and assign a RepNotify
	* 2. Register the variable for replication in GetLifetimeReplicatedProps
	* 3. Notify the Ability System of the replication with GAMEPLAYATTRIBUTE_REPNOTIFY (see .cpp file)
	*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Method for clamping
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	/*
	* By mapping our Gameplay Tags to static function pointers of type FGameplayAttributes, we can hold static getter functions for each Gameplay Attribute in these function 
	* pointers and skip the usual process of declaring our own delegate signature and binding functions to it.
	*/
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/*
    * Attributes
    */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armour, Category = "Attributes")
	FGameplayAttributeData Armour;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armour);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmourShred, Category = "Attributes")
	FGameplayAttributeData ArmourShred;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmourShred);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritChance, Category = "Attributes")
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage, Category = "Attributes")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	/*
	* TODO: Change Mana to Souls for Vessel, implement resource tags to organise future class resources
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownReduction, Category = "Attributes")
	FGameplayAttributeData CooldownReduction;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CooldownReduction);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Attributes")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StunBuildup, Category = "Attributes")
	FGameplayAttributeData StunBuildup;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, StunBuildup);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageVulnerability, Category = "Attributes")
	FGameplayAttributeData DamageVulnerability;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, DamageVulnerability);

	UFUNCTION()
	void OnRep_Armour(const FGameplayAttributeData& OldArmour) const;

	UFUNCTION()
	void OnRep_ArmourShred(const FGameplayAttributeData& OldArmourShred) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const;

	UFUNCTION()
	void OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const;
	
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;

	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;

	UFUNCTION()
	void OnRep_StunBuildup(const FGameplayAttributeData& OldStunBuildup) const;

	UFUNCTION()
	void OnRep_DamageVulnerability(const FGameplayAttributeData& OldDamageVulnerability) const;

	/*
	* Damage Types
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalDamage, Category = "Damage Types")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireDamage, Category = "Damage Types")
	FGameplayAttributeData FireDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningDamage, Category = "Damage Types")
	FGameplayAttributeData LightningDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShadowDamage, Category = "Damage Types")
	FGameplayAttributeData ShadowDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ShadowDamage);

	UFUNCTION()
	void OnRep_FireDamage(const FGameplayAttributeData& OldFireDamage) const;

	UFUNCTION()
	void OnRep_LightningDamage(const FGameplayAttributeData& OldLightningDamage) const;

	UFUNCTION()
	void OnRep_ShadowDamage(const FGameplayAttributeData& OldShadowDamage) const;

	UFUNCTION()
	void OnRep_PhysicalDamage(const FGameplayAttributeData& OldPhysicalDamage) const;

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
	* Input Charge Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ChargeLMB, Category = "InputCharges")
	FGameplayAttributeData ChargeLMB;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ChargeLMB);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxChargeLMB, Category = "InputCharges")
	FGameplayAttributeData MaxChargeLMB;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxChargeLMB);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ChargeRMB, Category = "InputCharges")
	FGameplayAttributeData ChargeRMB;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ChargeRMB);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxChargeRMB, Category = "InputCharges")
	FGameplayAttributeData MaxChargeRMB;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxChargeRMB);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charge1, Category = "InputCharges")
	FGameplayAttributeData Charge1;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Charge1);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCharge1, Category = "InputCharges")
	FGameplayAttributeData MaxCharge1;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxCharge1);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charge2, Category = "InputCharges")
	FGameplayAttributeData Charge2;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Charge2);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCharge2, Category = "InputCharges")
	FGameplayAttributeData MaxCharge2;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxCharge2);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charge3, Category = "InputCharges")
	FGameplayAttributeData Charge3;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Charge3);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCharge3, Category = "InputCharges")
	FGameplayAttributeData MaxCharge3;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxCharge3);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charge4, Category = "InputCharges")
	FGameplayAttributeData Charge4;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Charge4);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCharge4, Category = "InputCharges")
	FGameplayAttributeData MaxCharge4;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxCharge4);

	UFUNCTION()
	void OnRep_ChargeLMB(const FGameplayAttributeData& OldChargeLMB) const;

	UFUNCTION()
	void OnRep_MaxChargeLMB(const FGameplayAttributeData& OldMaxChargeLMB) const;

	UFUNCTION()
	void OnRep_ChargeRMB(const FGameplayAttributeData& OldChargeRMB) const;

	UFUNCTION()
	void OnRep_MaxChargeRMB(const FGameplayAttributeData& OldMaxChargeRMB) const;

	UFUNCTION()
	void OnRep_Charge1(const FGameplayAttributeData& OldCharge1) const;

	UFUNCTION()
	void OnRep_MaxCharge1(const FGameplayAttributeData& OldMaxCharge1) const;

	UFUNCTION()
	void OnRep_Charge2(const FGameplayAttributeData& OldCharge2) const;

	UFUNCTION()
	void OnRep_MaxCharge2(const FGameplayAttributeData& OldMaxCharge2) const;

	UFUNCTION()
	void OnRep_Charge3(const FGameplayAttributeData& OldCharge3) const;

	UFUNCTION()
	void OnRep_MaxCharge3(const FGameplayAttributeData& OldMaxCharge3) const;

	UFUNCTION()
	void OnRep_Charge4(const FGameplayAttributeData& OldCharge4) const;

	UFUNCTION()
	void OnRep_MaxCharge4(const FGameplayAttributeData& OldMaxCharge4) const;

	static FGameplayTag GetMaxChargeTagFromInput(const FGameplayTag& InputTag);
	static FGameplayTag GetCurrentChargeTagFromInput(const FGameplayTag& InputTag);

	/*
	* Summon Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "Summon Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Energy);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "Summon Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxEnergy);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Lifespan, Category = "Summon Attributes")
	FGameplayAttributeData Lifespan;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Lifespan);

	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldEnergy) const;

	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy) const;

	UFUNCTION()
	void OnRep_Lifespan(const FGameplayAttributeData& OldLifespan) const;

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
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void Debuff(const FEffectProperties& Props);
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void SendXPEvent(const FEffectProperties& Props);
	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};
