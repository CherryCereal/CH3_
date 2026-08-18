// Fill out your copyright notice in the Description page of Project Settings.

#include "HWPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"

AHWPawn::AHWPawn()
{
	PrimaryActorTick.bCanEverTick = true;

    // 1. CapsuleComponent - 루트 컴포넌트로 설정 (충돌 판정용)
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->InitCapsuleSize(34.f, 88.f);
    CapsuleComponent->SetSimulatePhysics(false);
    SetRootComponent(CapsuleComponent);

    // 2. SkeletalMeshComponent - 캡슐에 부착
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
    SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
    SkeletalMeshComponent->SetSimulatePhysics(false);
    SkeletalMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -88.f)); // 캡슐 바닥에 맞춤
    SkeletalMeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    // 3. SpringArmComponent - 캡슐에 부착
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(CapsuleComponent);
    SpringArmComponent->TargetArmLength = 400.f;
    SpringArmComponent->bUsePawnControlRotation = false; 
    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->CameraLagSpeed = 10.f;

    // 4. CameraComponent - SpringArm 끝(소켓)에 부착
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false; 

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
}

void AHWPawn::BeginPlay()
{
	Super::BeginPlay();
}
void AHWPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    UE_LOG(LogTemp, Warning, TEXT("PossessedBy called"));

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        UE_LOG(LogTemp, Warning, TEXT("PC Cast Success"));

        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Subsystem Found"));

            if (DefaultMappingContext)
            {
                UE_LOG(LogTemp, Warning, TEXT("IMC Valid, Adding..."));
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is NULL!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Subsystem NOT Found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PC Cast FAILED!"));
    }
}

void AHWPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    ApplyRotation(DeltaTime);
    ApplyMovement(DeltaTime);

    CachedLookInput = FVector2D::ZeroVector;
}

void AHWPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHWPawn::Move);
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AHWPawn::Move);
        }

        // ★ LookAction 유효성 확인 로그
        if (LookAction)
        {
            UE_LOG(LogTemp, Warning, TEXT("LookAction Valid, Binding..."));
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHWPawn::Look);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("LookAction is NULL! Check BP_HWPawn Class Defaults"));
        }
    }
}

void AHWPawn::Move(const FInputActionValue& Value)
{
    CachedMoveInput = Value.Get<FVector2D>();
}

void AHWPawn::Look(const FInputActionValue& Value)
{
    CachedLookInput = Value.Get<FVector2D>();

    UE_LOG(LogTemp, Warning, TEXT("Look Input: X=%f, Y=%f"), CachedLookInput.X, CachedLookInput.Y);
}

void AHWPawn::ApplyMovement(float DeltaTime)
{
    if (CachedMoveInput.IsNearlyZero())
    {
        return;
    }

    // Pawn의 로컬 축 기준 오프셋 계산 (X = Forward, Y = Right)
    const float DeltaForward = CachedMoveInput.Y * MoveSpeed * DeltaTime;
    const float DeltaRight = CachedMoveInput.X * MoveSpeed * DeltaTime;

    const FVector LocalOffset(DeltaForward, DeltaRight, 0.f);

    // Sweep = true : 이동 중 충돌 감지 (벽 뚫고 지나가지 않도록)
    AddActorLocalOffset(LocalOffset, /*bSweep=*/true);
}

void AHWPawn::ApplyRotation(float DeltaTime)
{
    if (CachedLookInput.IsNearlyZero())
    {
        return;
    }

    // ---- Yaw: Pawn 루트(Capsule) 전체를 좌우로 회전 → 이동 방향도 같이 회전 ----
    const float DeltaYaw = CachedLookInput.X * YawRotationSpeed * DeltaTime;
    if (!FMath::IsNearlyZero(DeltaYaw))
    {
        AddActorLocalRotation(FRotator(0.f, DeltaYaw, 0.f));
    }

    // ---- Pitch: SpringArm만 상하로 회전 → Pawn은 평면 유지, 카메라만 위아래를 봄 ----
    const float DeltaPitch = -CachedLookInput.Y * PitchRotationSpeed * DeltaTime; // 위로 마우스 올리면 위를 보도록 부호 반전
    if (!FMath::IsNearlyZero(DeltaPitch))
    {
        CurrentPitch = FMath::Clamp(CurrentPitch + DeltaPitch, MinPitch, MaxPitch);

        FRotator NewSpringArmRotation = SpringArmComponent->GetRelativeRotation();
        NewSpringArmRotation.Pitch = CurrentPitch;
        SpringArmComponent->SetRelativeRotation(NewSpringArmRotation);
    }
}
