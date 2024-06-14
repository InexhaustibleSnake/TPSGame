// This project is made for a test assignment

#include "Logic/GameModes/TPSGameModeDeathMatch.h"
#include "Characters/TPSPlayerCharacter.h"

ATPSGameModeDeathMatch::ATPSGameModeDeathMatch()
{
    DefaultPawnClass = ATPSPlayerCharacter::StaticClass();
}
