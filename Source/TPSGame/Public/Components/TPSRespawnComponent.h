// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSRespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPSGAME_API UTPSRespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSRespawnComponent();

    void Respawn();

protected:
    void RespawnTimerUpdate();

    bool IsRespawnInProgress() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Respawn")
    int32 RespawnTime = 5;

private:
    FTimerHandle RespawnTimerHandle;

    int32 RespawnCountDown = 5;
};
