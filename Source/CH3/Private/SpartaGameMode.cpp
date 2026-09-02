#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "HWPawn.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = AHWPawn::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}