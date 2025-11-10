// Copyright Druid Mechanics


#include "Checkpoint/Checkpoint.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interaction/PlayerInterface.h"

ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) // Need the same syntax as found in the APlayerStart Constructor
{
	PrimaryActorTick.bCanEverTick = false;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Align the bottom of the CheckpointMesh with the PlayerStart Capsule Component and add an X-offset so the Player haracter doesn't spawn inside the mesh
	const float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CheckpointMesh->SetRelativeLocation(FVector(350.f, 0.f, -CapsuleHalfHeight)); 
	Sphere->SetSphereRadius(350.f);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}

void ACheckpoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Create and set a Dynamic Material Instance from the Checkpoint's material, so we can later set the material's "glow" variable dynamically
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckpointReached(DynamicMaterialInstance);
}