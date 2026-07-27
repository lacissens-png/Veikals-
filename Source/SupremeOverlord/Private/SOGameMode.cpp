#include "SOGameMode.h"

#include "SOCharacter.h"
#include "SOHUD.h"
#include "SOPlayerController.h"

ASOGameMode::ASOGameMode()
{
	DefaultPawnClass      = ASOCharacter::StaticClass();
	PlayerControllerClass = ASOPlayerController::StaticClass();
	HUDClass              = ASOHUD::StaticClass();
}
