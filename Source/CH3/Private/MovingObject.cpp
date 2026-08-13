// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingObject.h"

// Sets default values
AMovingObject::AMovingObject()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();

	MoveSpeed = FMath::RandRange(100.0f, 500.0f);
	MaxRange = FMath::RandRange(500.0f, 1000.0f);

	FVector Origin = FVector(500.0f, 500.0f, 0.0f);

	FVector Extent = FVector(200.0f, 200.0f, 0.0f);
	FVector RandomLocation = FMath::RandPointInBox(FBox(Origin - Extent, Origin + Extent));

	SetActorLocation(RandomLocation);
}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MoveSpeed))
	{
		AddActorLocalOffset(FVector(MoveSpeed * DeltaTime, 0, 0));
	}
	if (FMath::Abs(GetActorLocation().X - 500.0f) > MaxRange)
	{
		MoveSpeed *= -1;
	}
}

