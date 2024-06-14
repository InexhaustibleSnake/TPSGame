// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Logic/GameModes/TPSBaseGameMode.h"
#include "TPSGameModeDeathMatch.generated.h"

UCLASS()
class TPSGAME_API ATPSGameModeDeathMatch : public ATPSBaseGameMode
{
    GENERATED_BODY()

public:
    ATPSGameModeDeathMatch();
};
