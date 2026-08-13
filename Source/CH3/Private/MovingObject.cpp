// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingObject.h"

// Sets default values
AMovingObject::AMovingObject()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Shapes/Shape_Torus.Shape_Torus"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Rust.M_Metal_Rust"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}

	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 100.0f;
	MaxRange = 300.0f;
}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector(500.0f, 100.0f, 0.0f));
}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MoveSpeed))
	{
		AddActorLocalOffset(FVector(MoveSpeed * DeltaTime, 0, 0));
	}
	if (FVector::Dist(GetActorLocation(), FVector(500.0f, 100.0f, 0.0f)) > MaxRange)
	{
		MoveSpeed *= -1;
	}
}

