
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}


void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if(PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
			}
			if(PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &ASpartaCharacter::StartJump);
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopJump);
			}
			if(PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
			}
			if(PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Started, this, &ASpartaCharacter::StartSprint);
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
			}
		}
	}
}

void ASpartaCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MoveInput = Value.Get<FVector2D>();

	if(!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& Value)
{
	Jump();
}
void ASpartaCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}
void ASpartaCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}
void ASpartaCharacter::StartSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void ASpartaCharacter::StopSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}