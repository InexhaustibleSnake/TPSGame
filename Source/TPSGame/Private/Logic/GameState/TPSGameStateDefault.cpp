// This project is made for a test assignment

#include "Logic/GameState/TPSGameStateDefault.h"
#include "Net/UnrealNetwork.h"
#include "Logic/GameModes/TPSGameModeDeathMatch.h"

void ATPSGameStateDefault::BeginPlay()
{
    Super::BeginPlay();

    StartTimer();
}

void ATPSGameStateDefault::RestartGame()
{
    SetMatchState(EMatchState::Started);
}

void ATPSGameStateDefault::StartTimer()
{
    RemainingMatchTime = MatchTime;

    GetWorldTimerManager().SetTimer(MatchTimer, this, &ATPSGameStateDefault::GameTimerUpdate, 1.0f, true);
}

void ATPSGameStateDefault::GameTimerUpdate()
{
    RemainingMatchTime = FMath::Clamp(--RemainingMatchTime, 0, MatchTime);

    if (!FMath::IsNearlyZero(RemainingMatchTime)) return;

    GetWorldTimerManager().ClearTimer(MatchTimer);

    SetMatchState(EMatchState::Ended);
}

void ATPSGameStateDefault::ResetTimer()
{
    RemainingMatchTime = MatchTime;

    StartTimer();
}

void ATPSGameStateDefault::SetMatchState(const EMatchState NewState)
{
    CurrentMatchState = NewState;
    OnRep_MatchState();
}

void ATPSGameStateDefault::OnRep_MatchState()
{
    OnMatchStateChanged.Broadcast(CurrentMatchState);

    if (CurrentMatchState == EMatchState::Started)
    {
        ResetTimer();
    }
}

void ATPSGameStateDefault::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ATPSGameStateDefault, RemainingMatchTime, COND_InitialOnly);
    DOREPLIFETIME_CONDITION(ATPSGameStateDefault, CurrentMatchState, COND_InitialOnly);
}
