// This project is made for a test assignment

#include "Components/TPSRespawnComponent.h"
#include "Logic/GameModes/TPSGameModeDeathMatch.h"

UTPSRespawnComponent::UTPSRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    RespawnCountDown = RespawnTime;
}

void UTPSRespawnComponent::Respawn()
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UTPSRespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

void UTPSRespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountDown == 0)
    {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = Cast<ATPSGameModeDeathMatch>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->RequestRespawn(Cast<AController>(GetOwner()));

        RespawnCountDown = RespawnTime;
    }
}

bool UTPSRespawnComponent::IsRespawnInProgress() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}
