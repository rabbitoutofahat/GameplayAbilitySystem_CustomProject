// Copyright Druid Mechanics


#include "Actor/AuraMagicCircle.h"
#include "Components/DecalComponent.h"

AAuraMagicCircle::AAuraMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AAuraMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAuraMagicCircle::BeginPlay()
{
	Super::BeginPlay();

}