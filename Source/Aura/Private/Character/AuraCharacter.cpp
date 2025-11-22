// Copyright Druid Mechanics


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Actor/AuraMagicCircle.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AAuraCharacter::AAuraCharacter()
{
	// Common parameters for a top-down game
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	CharacterClass = ECharacterClass::Elementalist;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server, load progress from the disk
	InitAbilityActorInfo();
	LoadProgress();

	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameMode->LoadWorldState(GetWorld());
	}
}

void AAuraCharacter::LoadProgress()
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadIn)
		{
			InitialiseDefaultAttributes(); // We want to initialise default attributes by applying it as a gameplay effect to the character at the beginning of the game
			AddCharacterAbilities();
		}
		else
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}

			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			UAuraAbilitySystemLibrary::InitialiseAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointReward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 InLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointReward;
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		// Save CheckpointTag to Game Instance
		SaveData->PlayerStartTag = CheckpointTag;

		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}

		if (UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
		{
			SaveData->Strength = AuraAttributeSet->GetStrength();
			SaveData->Intelligence = AuraAttributeSet->GetIntelligence();
			SaveData->Resilience = AuraAttributeSet->GetResilience();
			SaveData->Vigor = AuraAttributeSet->GetVigor();
		}

		SaveData->bFirstTimeLoadIn = false; // Set boolean to false so we don't initialise the character with default Primary Attributes on next load-in

		if (!HasAuthority()) return; // UAuraAbilitySystemLibrary::GetAbilityInfo only returns AbilityInfo on the server because only the game mode has it

		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		SaveData->SavedAbilities.Empty(); // If we've changed abilities, they might not be added if they already exist in some form in SavedAbilities
		FForEachAbilitySignature SaveAbilityDelegate;
		SaveAbilityDelegate.BindLambda(
			[this, AuraASC, &SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
				FAuraAbilityInfo AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this)->FindAbilityInfoForTag(AbilityTag);
				
				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = AbilityInfo.Ability;
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
				SavedAbility.AbilityInput = AuraASC->GetInputFromAbilityTag(AbilityTag);
				SavedAbility.AbilityType = AbilityInfo.AbilityType;
				SavedAbility.AbilityLevel = AbilitySpec.Level;

				SaveData->SavedAbilities.AddUnique(SavedAbility); // Need to overload FSavedAbility's == operator for AddUnique to work
			});
		AuraASC->ForEachAbility(SaveAbilityDelegate); // Fill in SaveData's SavedAbility TArray by executing the lambda above "for each ability"

		AuraGameMode->SaveInGameProgressData(SaveData);
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda(
		[this]()
		{
			AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
			if (AuraGM) AuraGM->PlayerDied(this);
		});
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DeathTimerDelegate, DeathTimer, false); // Start the death timer (set at 5 seconds by default); when the timer depletes, broadcast the lambda above

	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // Don't want the camera to just fall / travel with the ragdolling mesh
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);

		// Adding/Removing Loose tags since we're only replicated one boolean
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate(); // Client-side
		}
		else 
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned) BurnDebuffComponent->Activate();
	else BurnDebuffComponent->Deactivate();
}

ASummonCharacter* AAuraCharacter::SpawnSummonedMinion(UClass* Class, const FTransform& SpawnTransform, int32 OwnerLevel)
{
	ASummonCharacter* Summon = GetWorld()->SpawnActorDeferred<ASummonCharacter>(Class, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Summon->OwnerActor = this;
	Summon->SetLevel(OwnerLevel); // Set after SpawnSummonedMinion is called in blueprint
	Summon->FinishSpawning(SpawnTransform);
	Summon->SpawnDefaultController();
	return Summon;
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this); // Input params are owner actor (for mixed replication mode this must be the controller), avatar actor
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet(); // For binding gameplay effect delegates

	// Can put these lines on AuraCharacterBase instead, but then remember to call Super after the above code (i.e., after ASC is valid)
	OnASCRegistered.Broadcast(AbilitySystemComponent); // For Debuff Niagara Component
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
