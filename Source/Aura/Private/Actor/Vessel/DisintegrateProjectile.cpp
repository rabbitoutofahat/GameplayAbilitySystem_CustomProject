// Roland Frances


#include "Actor/Vessel/DisintegrateProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Character/PlayableClasses/Vessel.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ADisintegrateProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	DamageEffectParams.RadialDamageOrigin = OtherActor->GetActorLocation();

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	AttachToActor(OtherActor, AttachmentRules);
	
	ProjectileMovement->SetComponentTickEnabled(false);
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->bAutoActivate = false;
	SetActorEnableCollision(false);
	
	FTimerHandle DetonationTimerHandle;
	GetWorldTimerManager().SetTimer(DetonationTimerHandle, this, &ADisintegrateProjectile::DealRadialDamageToEnemies, DetonationTimer, false);
}

void ADisintegrateProjectile::DealRadialDamageToEnemies()
{
	if (!bHit) PlayImpactEffects();
	if (HasAuthority())
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Cast<AVessel>(DamageEffectParams.WorldContextObject));
		TArray<AActor*> ActorsToDamage;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(DamageEffectParams.WorldContextObject, ActorsToDamage, ActorsToIgnore, DamageEffectParams.RadialDamageOuterRadius, DamageEffectParams.RadialDamageOrigin);

		for (AActor* Actor : ActorsToDamage)
		{
			if (!Actor->ActorHasTag(FName("Enemy"))) continue;
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
			{
				const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulse;
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UAuraAbilitySystemLibrary::ApplyDamageEffectToTarget(DamageEffectParams);
			}
		}
		Destroy();
	}
	else bHit = true;
}
