// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "HWPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class CH3_API AHWPawn : public APawn
{
	GENERATED_BODY()

public:
	AHWPawn();

protected:
	virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// ---------- 컴포넌트 ----------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCapsuleComponent> CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> CameraComponent;

	// ---------- 입력 관련 변수 ----------
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    // ---------- 이동/회전 설정값 ----------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 300.f; // cm/s

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float YawRotationSpeed = 100.f; // deg/s (입력 1.0 기준)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float PitchRotationSpeed = 100.f; // deg/s (입력 1.0 기준)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "-89", ClampMax = "0"))
    float MinPitch = -80.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0", ClampMax = "89"))
    float MaxPitch = 80.f;

    // ---------- 입력 캐시 (프레임 독립적 처리를 위해 Tick에서 소비) ----------
    FVector2D CachedMoveInput = FVector2D::ZeroVector;
    FVector2D CachedLookInput = FVector2D::ZeroVector;

    // 현재 SpringArm Pitch 누적값 (클램프 계산용)
    float CurrentPitch = 0.f;

    // ---------- 입력 처리 함수 ----------
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    // ---------- 실제 이동/회전 적용 ----------
    void ApplyMovement(float DeltaTime);
    void ApplyRotation(float DeltaTime);
};
