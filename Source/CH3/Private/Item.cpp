
#include "Item.h"


AItem::AItem()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 90.0f;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	FVector Origin = FVector(200.0f, 0.0f, 25.0f);

	FVector Extent = FVector(1000.0f, 1000.0f, 0.0f);
	FVector RandomLocation = FMath::RandPointInBox(FBox(Origin - Extent, Origin + Extent));

	SetActorLocation(RandomLocation);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}


