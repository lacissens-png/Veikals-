#include "SOGameMode.h"

#include "SOCharacter.h"
#include "SOPlayerController.h"

ASOGameMode::ASOGameMode()
{
	DefaultPawnClass      = ASOCharacter::StaticClass();
	PlayerControllerClass = ASOPlayerController::StaticClass();
}
