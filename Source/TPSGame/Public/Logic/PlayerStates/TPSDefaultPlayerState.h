// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSDefaultPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, NewScore);

UCLASS()
class TPSGAME_API ATPSDefaultPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "PlayerState")
    FOnScoreChangedSignature OnScoreChanged;

    void AddScores(float Amount);  // Default SetScore doesn't trigger OnRep_Score()

    void ClearScore();

protected:
    virtual void OnRep_Score() override;
};
