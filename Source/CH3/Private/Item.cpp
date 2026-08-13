
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

	GetWorldTimerManager().SetTimer(
		RelocateTimerHandle,
		this,
		&AItem::RelocateRandomly,
		3.0f,
		true // 3초마다 반복
	);
}

void AItem::RelocateRandomly()
{
	FVector Origin = FVector(200.0f, 0.0f, 25.0f);
	FVector Extent = FVector(1000.0f, 1000.0f, 0.0f);

	float RandomX = FMath::RandRange(Origin.X - Extent.X, Origin.X + Extent.X);
	float RandomY = FMath::RandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y);

	SetActorLocation(FVector(RandomX, RandomY, Origin.Z));
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}


