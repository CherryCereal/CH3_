#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "HWPawn.h"
#include "SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = AHWPawn::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
}
