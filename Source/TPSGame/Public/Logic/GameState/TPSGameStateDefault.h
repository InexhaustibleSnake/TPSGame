// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TPSGameStateDefault.generated.h"

UENUM(BlueprintType)
enum class MatchState : uint8
{
    Started,
    Ended
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, MatchState, NewState);

UCLASS()
class TPSGAME_API ATPSGameStateDefault : public AGameStateBase
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "AIRGameState")
    void StartTimer();

    UFUNCTION(BlueprintCallable, Category = "AIRGameState")
    float GetMatchRemainingTime() const { return RemainingMatchTime; }

    UFUNCTION(BlueprintCallable, Category = "AIRGameState")
    void RestartGame();

    UPROPERTY(BlueprintAssignable, Category = "AIRGameState")
    FOnMatchStateChangedSignature OnMatchStateChanged;

protected:
    virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void GameTimerUpdate();

    void ResetTimer();

    void SetMatchState(const MatchState NewState);

    UFUNCTION()
    void OnRep_MatchState();

    UPROPERTY(EditDefaultsOnly, meta = (Units = "Seconds"), meta = (ClampMin = "10"))
    float MatchTime = 60.0f;

    UPROPERTY(Replicated, meta = (Units = "Seconds"))
    float RemainingMatchTime = 10.0f;

    UPROPERTY(ReplicatedUsing = OnRep_MatchState)
    MatchState CurrentMatchState;

    FTimerHandle MatchTimer;
};
