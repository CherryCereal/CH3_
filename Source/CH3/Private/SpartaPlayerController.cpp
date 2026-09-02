#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ASpartaPlayerController::ASpartaPlayerController()
{
	InputMappingContext = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	JumpAction = nullptr;
	SprintAction = nullptr;
	HUDWidgetClass = nullptr;
	HUDWidgetInstance = nullptr;
	MainMenuWidgetClass = nullptr;
	MainMenuWidgetInstance = nullptr;
	GameOverWidgetClass = nullptr;
	GameOverWidgetInstance = nullptr;
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if(InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu();
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowMainMenu()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (GameOverWidgetInstance) 
	{ 
		GameOverWidgetInstance->RemoveFromParent(); 
		GameOverWidgetInstance = nullptr; 
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ASpartaPlayerController::ShowGameOverScreen()
{
	if (HUDWidgetInstance) 
	{ 
		HUDWidgetInstance->RemoveFromParent(); 
		HUDWidgetInstance = nullptr; 
	}
	if (MainMenuWidgetInstance) 
	{ 
		MainMenuWidgetInstance->RemoveFromParent(); 
		MainMenuWidgetInstance = nullptr; 
	}

	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	}

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText"))))
		{
			if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)));
			}
		}

		if (UFunction* PlayAnimFunc = GameOverWidgetInstance->FindFunction(FName("PlayGameOverAnim")))
		{
			GameOverWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}
	}
}
void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverWidgetInstance) 
	{ 
		GameOverWidgetInstance->RemoveFromParent(); 
		GameOverWidgetInstance = nullptr; 
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
			if (SpartaGameState)
			{
				SpartaGameState->UpdateHUD();
			}
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast <USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}

void ASpartaPlayerController::RestartGame()
{
	StartGame();
}

void ASpartaPlayerController::GoToMainMenu()
{
	SetPause(false);
	UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
}

void ASpartaPlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}