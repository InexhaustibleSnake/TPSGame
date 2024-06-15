// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Logic/GameModes/TPSBaseGameMode.h"
#include "TPSGameModeDeathMatch.generated.h"

class ATPSDefaultPlayerState;

UCLASS()
class TPSGAME_API ATPSGameModeDeathMatch : public ATPSBaseGameMode
{
    GENERATED_BODY()

public:
    ATPSGameModeDeathMatch();

    void OnPlayerKilled(AController* KillerController, AController* VictimController);

    void RequestRespawn(AController* Controller);

protected:
    void RespawnOnePlayer(AController* Controller);

    void StartRespawn(AController* Controller);

    void AddPlayerScores(AController* Controller, float Amount);
};
