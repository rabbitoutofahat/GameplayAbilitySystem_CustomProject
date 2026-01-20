// Copyright Druid Mechanics


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitialiseNativeGameplayTags()
{
	// Declare native Gameplay Tags through the Gameplay Tag Manager
	
	/*
	* Primary Attributes
	*/
	//GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases physical damage"));
	//GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	//GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases Armour and Armour Penetration"));
	//GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases Health"));

	/*
	* Attributes
	*/
	GameplayTags.Attributes_Armour = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Armour"), FString("Reduces incoming damage"));
	GameplayTags.Attributes_ArmourShred = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.ArmourShred"), FString("Reduces enemy Armour"));
	GameplayTags.Attributes_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.CriticalHitChance"), FString("Chance to deal critical hits"));
	GameplayTags.Attributes_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.CriticalHitDamage"), FString("Bonus damage dealt on critical hits"));
	//GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces Critical Hit Chance of attacking enemies"));
	GameplayTags.Attributes_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.HealthRegeneration"), FString("Amount of Health regenerated per second"));
	//GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of Mana regenerated per second"));
	GameplayTags.Attributes_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.MaxHealth"), FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.MaxMana"), FString("Maximum amount of Mana obtainable"));
	
	GameplayTags.Attributes_Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Damage.Physical"), FString("Physical damage modifier"));
	GameplayTags.Attributes_Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Damage.Fire"), FString("Fire damage modifier"));
	GameplayTags.Attributes_Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Damage.Lightning"), FString("Lightning damage modifier"));
	GameplayTags.Attributes_Damage_Shadow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Damage.Shadow"), FString("Shadow damage modifier"));
	GameplayTags.Attributes_CooldownReduction = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.CooldownReduction"), FString("Reduces ability cooldowns"));
	GameplayTags.Attributes_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.AttackSpeed"), FString("Increases attack speed"));
	GameplayTags.Attributes_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.MovementSpeed"), FString("Increases movement speed"));
	GameplayTags.Attributes_StunBuildup = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.StunBuildup"), FString("Increases stun buildup on enemies"));
	GameplayTags.Attributes_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.BlockChance"), FString("Chance to cut incoming damage in half"));

	/*
	* Vital Attributes
	*/
	GameplayTags.Attributes_Vital_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Energy"), FString("Energy resource for abilities"));

	/*
    * Damage Resistance Attributes
    */
	/*GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Reduces incoming Fire damage"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Reduces incoming Lightning damage"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Reduces incoming Arcane damage"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Reduces incoming Physical damage"));*/

	/*
	* Meta Attributes
	*/
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("Incoming XP Meta Attribute"));

	/*
	* Input Tags
	*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag for Right Mouse Button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag for the 1 key"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag for the 2 key"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input Tag for the 3 key"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input Tag for the 4 key"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"), FString("Input Tag for Passive Ability 1"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"), FString("Input Tag for Passive Ability 2"));

	/*
 	* Damage Tags
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage Type"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage Type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage Type"));
	GameplayTags.Damage_Shadow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Shadow"), FString("Shadow Damage Type"));

	/*
	* Debuffs
	*/
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"), FString("Debuff for Fire Damage"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"), FString("Debuff for Lightning Damage"));
	//GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"), FString("Debuff for Arcane Damage"));
	//GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"), FString("Debuff for Physical Damage"));

	/*
	* Debuff Parameters
	*/
	GameplayTags.Debuff_Params_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Params.Chance"), FString("Chance to apply the debuff"));
	GameplayTags.Debuff_Params_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Params.Damage"), FString("Damage applied by the debuff"));
	GameplayTags.Debuff_Params_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Params.Duration"), FString("Duration of the debuff"));
	GameplayTags.Debuff_Params_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Params.Frequency"), FString("Tick rate of the debuff"));

	/*
    * Abilities
    */
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString("Empty Aura Ability Tag"));
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Attack Ability Tag"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Summon Ability Tag"));
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("Hit React Ability Tag"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Ability is locked and cannot be unlocked yet"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Ability is eligible to be unlocked"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Ability has been unlocked"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Ability is currently equipped"));

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"), FString("Type Offensive"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Type Passive"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Type None"));

	/*
	* Offensive Spells
	*/
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("FireBolt Ability Tag"));
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("FireBlast Ability Tag"));
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Electrocute Ability Tag"));
	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), FString("Arcane Shards Ability Tag"));

	/*
	* Passive Spells
	*/
	/*GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HaloOfProtection"), FString("Halo Of Protection Passive Ability Tag"));
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.LifeSiphon"), FString("Life Siphon Passive Ability Tag"));
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaSiphon"), FString("Mana Siphon Passive Ability Tag"));*/

	/*
    * Combat Socket Tags
    */
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Weapon"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Right Hand"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Left Hand"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("Tail"));

	/*
	* Montage Tags
	*/
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Attack 4"));

	/*
 	* Map of Damage Types to Resistances
	*/
	/*GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);*/

	/*
	* Map of Damage Types to Debuffs
	*/
	/*GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);*/

	/*
	* Map of Damage Types to Damage Attributes
	*/
	GameplayTags.DamageTypesToAttributes.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Damage_Physical);
	GameplayTags.DamageTypesToAttributes.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Damage_Fire);
	GameplayTags.DamageTypesToAttributes.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Damage_Lightning);
	GameplayTags.DamageTypesToAttributes.Add(GameplayTags.Damage_Shadow, GameplayTags.Attributes_Damage_Shadow);

	/*
    * Effects
    */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Grant to enemies when they are hit by an ability"));

	/*
	* Player Tags
	*/
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), FString("Block Input Pressed callback for input"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), FString("Block Input Held callback for input"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), FString("Block Input Released callback for input"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Block tracing under the cursor"));

	/*
	* Gameplay Cues
	*/
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"), FString("Gameplay Cue for Fire Blast Ability"));
	GameplayTags.GameplayCue_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ArcaneShards"), FString("Gameplay Cue to spawn an Arcane Shard"));
	
	/*
	* Vessel Character Class
	*/
	GameplayTags.Abilities_Utility_Haunt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Utility.Haunt"), FString("Vessel Utility Ability Tag"));
	GameplayTags.Abilities_Passive_DemonicSoul = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.DemonicSoul"), FString("Vessel Starting Passive Tag"));
	GameplayTags.Abilities_SpecialAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.SpecialAttack"), FString("Special Attack Ability Tag for Summons"));
	GameplayTags.Abilities_Vessel_SoulSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.SoulSiphon"), FString("Vessel Starting Ability Tag"));
	GameplayTags.Abilities_Vessel_SummonDregling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.SummonDregling"), FString("Summon Dregling Ability Tag"));
	GameplayTags.Abilities_Vessel_SummonInfernalGolem = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.SummonInfernalGolem"), FString("Summon Infernal Golem Ability Tag"));
	GameplayTags.Abilities_Vessel_SummonChaosElemental = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.SummonChaosElemental"), FString("Summon Chaos Elemental Ability Tag"));
	GameplayTags.Abilities_Vessel_DemonfireRift = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.DemonfireRift"), FString("Demonfire Rift Ability Tag"));
	GameplayTags.Abilities_Vessel_Pandemonium = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.Pandemonium"), FString("Pandemonium Ultimate Ability Tag"));
	GameplayTags.Abilities_Vessel_Disintegrate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Vessel.Disintegrate"), FString("Disintegrate Ability Tag"));

	GameplayTags.GameplayCue_Rift_Haunt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Rift.Haunt"), FString("Gameplay Cue for Haunt Rift Effect"));
	GameplayTags.GameplayCue_Rift_Dregling = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Rift.Dregling"), FString("Gameplay Cue for Dregling Rift Effect"));
	GameplayTags.GameplayCue_Rift_InfernalGolem = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Rift.InfernalGolem"), FString("Gameplay Cue for Infernal Golem Rift Effect"));
	GameplayTags.GameplayCue_Rift_ChaosElemental = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Rift.ChaosElemental"), FString("Gameplay Cue for Chaos Elemental Rift Effect"));
	GameplayTags.GameplayCue_Explosion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Explosion"), FString("Gameplay Cue for Explosion Effects"));

	GameplayTags.Attributes_MaxEnergy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxEnergy"), FString("Maximum amount of Energy obtainable"));
	GameplayTags.Buff_DemonicStrength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Buff.DemonicStrength"), FString("Given to Vessel's demons, allows one free use of their respective special ability"));
}
