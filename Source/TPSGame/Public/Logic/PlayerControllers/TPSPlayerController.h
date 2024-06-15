// This project is made for a test assignment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

class UTPSRespawnComponent;

UCLASS()
class TPSGAME_API ATPSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ATPSPlayerController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UTPSRespawnComponent> RespawnComponent;
};
