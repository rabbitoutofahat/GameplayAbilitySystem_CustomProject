// Copyright Druid Mechanics


#include "Actor/AuraPointCollection.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AAuraPointCollection::AAuraPointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_0"));
	Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_1"));
	Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_2"));
	Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_3"));
	Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_4"));
	Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_5"));
	Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_6"));
	Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_7"));
	Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_8"));
	Pt_9 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_9"));
	Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_10"));

	ImmutablePts.Add(Pt_0);
	ImmutablePts.Add(Pt_1);
	ImmutablePts.Add(Pt_2);
	ImmutablePts.Add(Pt_3);
	ImmutablePts.Add(Pt_4);
	ImmutablePts.Add(Pt_5);
	ImmutablePts.Add(Pt_6);
	ImmutablePts.Add(Pt_7);
	ImmutablePts.Add(Pt_8);
	ImmutablePts.Add(Pt_9);
	ImmutablePts.Add(Pt_10);

	SetRootComponent(Pt_0); // Set Pt_0 as the Actor's Location, the Point Collection center
	Pt_1->SetupAttachment(GetRootComponent());
	Pt_2->SetupAttachment(GetRootComponent());
	Pt_3->SetupAttachment(GetRootComponent());
	Pt_4->SetupAttachment(GetRootComponent());
	Pt_5->SetupAttachment(GetRootComponent());
	Pt_6->SetupAttachment(GetRootComponent());
	Pt_7->SetupAttachment(GetRootComponent());
	Pt_8->SetupAttachment(GetRootComponent());
	Pt_9->SetupAttachment(GetRootComponent());
	Pt_10->SetupAttachment(GetRootComponent());
}

TArray<USceneComponent*> AAuraPointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access ImmutablePts out of bounds."));

	TArray<USceneComponent*> ArrayCopy; // Don't want to accidentally mess with the original ImmutablePts array

	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint); // Reposition this point such that it's rotated around Pt_0 by YawOverride degrees
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation()); 
		
		// Trace to the ground ignoring enemy actors within the Magic Circle
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(
			HitResult,
			RaisedLocation,
			LoweredLocation,
			TEXT("BlockAll"),
			QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal)); // Rotate the point to align with the ground normal, so any resulting Niagara systems are orthogonal to the surface being traced on

		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

void AAuraPointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}