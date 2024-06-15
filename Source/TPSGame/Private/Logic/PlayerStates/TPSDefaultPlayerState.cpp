// This project is made for a test assignment

#include "Logic/PlayerStates/TPSDefaultPlayerState.h"

void ATPSDefaultPlayerState::AddScores(float Amount)
{
    SetScore(GetScore() + Amount);
    OnRep_Score();
}

void ATPSDefaultPlayerState::ClearScore()
{
    SetScore(0);
    OnRep_Score();
}

void ATPSDefaultPlayerState::OnRep_Score()
{
    OnScoreChanged.Broadcast(GetScore());
}
