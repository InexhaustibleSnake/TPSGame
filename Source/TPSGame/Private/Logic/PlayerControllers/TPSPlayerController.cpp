// This project is made for a test assignment

#include "Logic/PlayerControllers/TPSPlayerController.h"
#include "Components/TPSRespawnComponent.h"

ATPSPlayerController::ATPSPlayerController() 
{
    RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");
}
