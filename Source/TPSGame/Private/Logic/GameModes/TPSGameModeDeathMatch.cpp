// This project is made for a test assignment

#include "Logic/GameModes/TPSGameModeDeathMatch.h"
#include "Characters/TPSPlayerCharacter.h"
#include "Logic/GameState/TPSGameStateDefault.h"
#include "Logic/PlayerStates/TPSDefaultPlayerState.h"
#include "Components/TPSRespawnComponent.h"

ATPSGameModeDeathMatch::ATPSGameModeDeathMatch()
{
    DefaultPawnClass = ATPSPlayerCharacter::StaticClass();
    GameStateClass = ATPSGameStateDefault::StaticClass();
    PlayerStateClass = ATPSDefaultPlayerState::StaticClass();
}

void ATPSGameModeDeathMatch::OnPlayerKilled(AController* KillerController, AController* VictimController)
{
    AddPlayerScores(KillerController, 1.0f);

    StartRespawn(VictimController);
}

void ATPSGameModeDeathMatch::RequestRespawn(AController* Controller)
{
    RespawnOnePlayer(Controller);
}

void ATPSGameModeDeathMatch::RespawnOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
}

void ATPSGameModeDeathMatch::StartRespawn(AController* Controller)
{
    if (!Controller) return;

    const auto RespawnComponent = Controller->FindComponentByClass<UTPSRespawnComponent>();
    if (!RespawnComponent) return;

    RespawnComponent->Respawn();
}

void ATPSGameModeDeathMatch::AddPlayerScores(AController* Controller, float Amount)
{
    if (!Controller) return;

    auto PlayerState = Controller->GetPlayerState<ATPSDefaultPlayerState>();
    if (!PlayerState) return;

    PlayerState->AddScores(1);
}
